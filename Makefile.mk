# This file is licensed under the the MIT License.
#
# Copyright (c) 2014 Matthijs Kooijman <matthijs@stdin.nl>
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#
# This Makefile snippet is intended to be included by other Makefiles.
# You can then either:
#  - include the $(ARDUINO_UNIX_SOURCES) in the build, or
#  - include the $(ARDUINO_UNIX_OBJECTS) in the link, or
#  - include the $(ARDUINO_UNIX_LIB_A) in the link
#
# You should also make your Makefile targets depend on the relevant
# files.
#
# In either case, you should put $(ARDUINO_UNIX_INCLUDE_PATH) in the
# include path for all sources compiled (including the
# ARDUINO_UNIX_SOURCES) and link with -lstdc++.

ARDUINO_UNIX_ROOT_PATH     := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
ARDUINO_UNIX_INCLUDE_PATH  := $(ARDUINO_UNIX_ROOT_PATH)/include
ARDUINO_UNIX_SRC_PATH      := $(ARDUINO_UNIX_ROOT_PATH)/src
ARDUINO_UNIX_CXX_SOURCES   := $(wildcard $(ARDUINO_UNIX_SRC_PATH)/*.cpp)
ARDUINO_UNIX_C_SOURCES     := $(wildcard $(ARDUINO_UNIX_SRC_PATH)/*.c)
ARDUINO_UNIX_SOURCES       := $(ARDUINO_UNIX_C_SOURCES) $(ARDUINO_UNIX_CXX_SOURCES)
ARDUINO_UNIX_C_OBJECTS     := $(ARDUINO_UNIX_C_SOURCES:.c=.o)
ARDUINO_UNIX_CXX_OBJECTS   := $(ARDUINO_UNIX_CXX_SOURCES:.cpp=.o)
ARDUINO_UNIX_OBJECTS       := $(ARDUINO_UNIX_C_OBJECTS) $(ARDUINO_UNIX_CXX_OBJECTS)
ARDUINO_UNIX_LIB_A         := $(ARDUINO_UNIX_ROOT_PATH)/ArduinoUnix.a

ARDUINO_UNIX_CFLAGS        += -I$(ARDUINO_UNIX_INCLUDE_PATH) -Wall
ARDUINO_UNIX_CXXFLAGS      += -I$(ARDUINO_UNIX_INCLUDE_PATH) -Wall
ARDUINO_UNIX_GCC           ?= gcc
ARDUINO_UNIX_AR            ?= ar

$(ARDUINO_UNIX_C_OBJECTS): %.o: %.c
	$(ARDUINO_UNIX_GCC) -c $(ARDUINO_UNIX_CPPFLAGS) $(ARDUINO_UNIX_CFLAGS) -o $@ $<

$(ARDUINO_UNIX_CXX_OBJECTS): %.o: %.cpp
	$(ARDUINO_UNIX_GCC) -c $(ARDUINO_UNIX_CPPFLAGS) $(ARDUINO_UNIX_CXXFLAGS) -o $@ $<

$(ARDUINO_UNIX_LIB_A): $(ARDUINO_UNIX_OBJECTS)
	rm -f $(ARDUINO_UNIX_LIB_A)
	$(ARDUINO_UNIX_AR) -qvc $(ARDUINO_UNIX_LIB_A) $(ARDUINO_UNIX_OBJECTS)

clean: clean-arduino-unix
clean-arduino-unix:
	rm -f $(ARDUINO_UNIX_OBJECTS) $(ARDUINO_UNIX_LIB_A)
