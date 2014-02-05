/*
  wiring.c - Functions related to timers and delays

  This file is licensed under the the MIT License.

  Copyright (c) 2014 Matthijs Kooijman <matthijs@stdin.nl>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

struct timespec startup_time;

#if _POSIX_MONOTONIC_CLOCK > 0
#define CLOCK_TO_USE CLOCK_MONOTONIC
#else
#define CLOCK_TO_USE CLOCK_REALTIME
#endif

#if _POSIX_CLOCK_SELECTION > 0 && _POSIX_TIMERS > 0
// We have both clock_gettime and clock_gettime available.
static void delay_internal(unsigned long seconds, unsigned long us)
{
  struct timespec time;
  if (clock_gettime(CLOCK_TO_USE, &time)) {
    fprintf(stderr, "clock_gettime failed: %s\n", strerror(errno));
    return;
  }
  time.tv_sec += seconds;
  time.tv_nsec += us * 1000UL;
  if (time.tv_nsec > 1E9) {
    time.tv_nsec -= 1E9;
    time.tv_sec += 1;
  }

  int res;
  do {
    res = clock_nanosleep(CLOCK_TO_USE, TIMER_ABSTIME, &time, NULL);
  } while (res == EINTR);

  if (res)
    fprintf(stderr, "clock_nanosleep failed: %s\n", strerror(res));
}

#else

// OSX doesn't provide clock_gettime and clock_nanosleep, OpenBSD
// doesn't provide clock_nanosleep.
// We'll have to implement the delay with nanosleep (where the actual
// delay can become longer if it gets interrupted often)
static void delay_internal(unsigned long seconds, unsigned long us)
{
  struct timespec time;
  time.tv_sec = seconds;
  time.tv_nsec = us * 1000UL;

  int res;
  do {
    res = nanosleep(&time, &time);
  } while (res == -1 && errno == EINTR);

  if (res)
    fprintf(stderr, "nanosleep failed: %s\n", strerror(errno));
}
#endif // _POSIX_CLOCK_SELECTION > 0 && _POSIX_TIMERS > 0

#if _POSIX_TIMERS > 0
  // Use clock_gettime if we have it
  static int gettime_internal(struct timespec *time)
  {
    return clock_gettime(CLOCK_TO_USE, time);
  }
#else
  // We can't guarantee we have clock_gettime, so use gettimeofday
  // instead. This is needed on OSX (which doesn't have clock_gettime)
  // and on OpenBSD (which has clock_gettime but doesn't implement the
  // complete POSIX "Timers option", so we can't easily detect it has
  // clock_gettime).
static int gettime_internal(struct timespec *time)
{
  struct timeval tv;
  int res = gettimeofday(&tv, NULL);
  if (!res) {
    time->tv_sec = tv.tv_sec;
    time->tv_nsec = tv.tv_usec * 1000UL;
  }
  return res;
}

#endif // _POSIX_TIMERS

// from http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
struct timespec time_diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1E9 +  end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp;
}

unsigned long millis()
{
  struct timespec current_time;
  gettime_internal(&current_time);
  struct timespec elapsed_time = time_diff(startup_time, current_time);
  return (elapsed_time.tv_sec * 1000UL) + (elapsed_time.tv_nsec / 1E6);
}

unsigned long micros() {
  struct timespec current_time;
  gettime_internal(&current_time);
  struct timespec elapsed_time = time_diff(startup_time, current_time);
  return (elapsed_time.tv_sec * 1E6) + (elapsed_time.tv_nsec / 1000UL);
}

void delay(unsigned long ms)
{
  delay_internal(ms / 1000L, (ms % 1000L) * 1000L);
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
  delay_internal(0, us);
}

void wiring_init()
{
  gettime_internal(&startup_time);
}

/* vim: set sw=2 sts=2 expandtab: */
