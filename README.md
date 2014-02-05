ArduinoUnix: Arduino sketches on Unix
=====================================
ArduinoUnix is an implementation of the Arduino API intended for
Unix-like systems.

It allows compiling Arduino sketches or libraries on a regular system.
Currently Linux, OSX and OpenBSD have been tested, but any UNIX- or
POSIX-like system could work.

The intended usecase is experimenting and testing non-hardware dependent
parts of sketches and libraries (both quick ad-hoc testing as well as
more structured unittesting).

What is (not) supported?
------------------------
 - I/O through stdin and stdout, by using the `StdioStream` object. The
   `Serial` object is an alias for this, to help existing sketches to
   work. If you need to do I/O to other places, there is the `FDStream`
   class that you can pass arbitrary file descriptors.

 - Delays (`delay()` and `delayMicroseconds()`) and timers (`millis()`
   and `micros()`) work normally.

 - `PROGMEM` related defines and functions (from `avr/pgmspace.h` are
   available), turning `PROGMEM` variables into normal variables and the
   various `pgm_load_` functions into nops.

 - The `IPAddress`, `Client` and `Server`, `String`, `Stream` and
   `Print` classes are available and work
   normallly.

 - The random number generator (`randomSeed()` and `random()`) work
   normally.

 - Some AVR-specific number-to-string conversion functions (`ultoa()`,
   `utoa()`, `ltoa()`, `itoa()` and `dtostrf()`) are available.

 - The `map()` and `makeWord()` functions are available.

 - The shifting functions (`shiftIn()` and `shiftOut()`) work normally
   (but they use `digitalRead()` and `digitalWrite()`, so they won't
   actually _do_ anything.

 - Digital pin I/O works to some degree: `pinMode()` and
   `digitalWrite()` update an internal state, which can be read back
   through `digitalRead()`.

 - Analog input (`analogReference()` and `analogRead()`) and PWM output (`analogWrite`) can be
   compiled, but do not have any effect.

 - Pulse input (`pulseIn()`) compiles, but will always wait for the
   timeout and then fail.

 - The tone functions (`tone()` and `noTone()`) compile, but do not have any
   effect.

 - Interrupts routines compile, but will never be called. This applies to both
   the external interrupts using the Arduino API (`attachInterrupt()`
   and `detachInterrupt()`) as well as low-level interrupts using the
   `ISR()` macro.

 - The pin mapping macros (`analogInPinToBit()`, `digitalPinToPort()`,
   `digitalPinToBitMask()`, `digitalPinToTimer()`, `analogInPinToBit()`,
   `portOutputRegister()`, `portInputRegister()` and `portModeRegister()`) are
   not available.

 - Direct register manipulation (e.g., using the AVR port registers) is
   not available. This means that any hardware that does not have an
   Arduino API available cannot be used.

Using ArduinoUnix
-----------------
To use ArduinoUnix, you'll have to set up your build process to:

 - Put the `include/` directory in the include path
 - Include the `.c` and `.cpp` files in the `src/` directory in the
   build.
 - Link against libstdc++ (pass `-lstdc++` to the linker command)

Furthermore, you should either:

 - Define a `setup()` and `loop()` function. `setup()` will be called
   once after startup, and `loop()` will be called indefinately
   afterwards, just like in a normal Arduino sketch.
   See `examples/HelloSketch` for an example.
 - Define a `main()` function like in a normal C program. In this case,
   you should make sure to call the `init()` function as soon as
   possible in your `main()` function to initialize ArduinoUnix.
   See `examples/HelloMain` for an example.

And, when you compile a `.ino` file, make sure to also pass `-x c++` to
let the compiler know it is a C++ source file and `-include Arduino.h`
to make sure `Arduino.h` is automatically included.

Using Makefiles
---------------
The easiest way of setting up the build is to use a Makefile.
ArduinoUnix supplies an `Makefile.mk` file that you can include in your
main Makefile which defines a number of useful variables and targets. If
you include `Makefile.mk`, you essentially have three ways to
incorporate ArduinoUnix in your build:

 1. Do compilation and linking of all source files in one command. In
    this case, make sure your make rule depends on
    `$(ARDUINO_UNIX_SOURCES)` and its commandline includes the
    `$(ARDUINO_UNIX_SOURCES)` and `-lstdc++`. For example:

		include ArduinoUnix/Makefile.mk

		my_program: my_program.cpp $(ARDUINO_UNIX_SOURCES)
			gcc my_program.cpp $(ARDUINO_UNIX_SOURCES) -I$(ARDUINO_UNIX_INCLUDE_PATH) -lstdc++ -o $@

 2. Do compilation of all source files separately and then link the .o
    files into the final binary. In this case, set up compilation of
    your own source files normally, passing `-I$(ARDUINO_INCLUDE_PATH)`.
    Then, make sure your link rule depends on `$(ARDUINO_UNIX_OBJECTS)`
    and its commandline includes `$(ARDUINO_UNIX_OBJECTS)` and
    `-lstdc++`. For example:

		include ArduinoUnix/Makefile.mk

		my_program: my_program.o $(ARDUINO_UNIX_OBJECTS)
			gcc my_program.o $(ARDUINO_UNIX_OBJECTS) -lstdc++ -o $@

		my_program.o: my_program.cpp
			gcc -c my_program.cpp -I$(ARDUINO_UNIX_INCLUDE_PATH) -o $@

 3. Compile the ArduinoUnix files into a static library (.a archive)
    file to be included in the link. In this case, set up compilation of
    your own source files normally, passing `-I$(ARDUINO_INCLUDE_PATH)`.
    Then, make sure your link rule depends on `$(ARDUINO_UNIX_LIB_A)`
    and its commandline includes `$(ARDUINO_UNIX_LIB_A)` and `-lstdc++`.
    For example:

		include ArduinoUnix/Makefile.mk

		my_program: my_program.o $(ARDUINO_UNIX_LIB_A)
			gcc my_program.o $(ARDUINO_UNIX_LIB_A) -lstdc++ -o $@

		my_program.o: my_program.cpp
			gcc -c my_program.cpp -I$(ARDUINO_UNIX_INCLUDE_PATH) -o $@

See also `examples/HelloMain/Makefile`, which also shows how to use each
of these three methods.

Licensing and copyright
-----------------------
This library collects code from a few different places, so the licensing
varies by file.

 - Some code is taken from the Arduino avr core, which is licensed under
   the GNU Lesser General Publice License, version 2.1 or above. The
   full text of this license can be found in the `LICENSE.LGPL` file.

 - Some code is taken from avr-libc, which is licensed under the
   3-clause BSD license.

 - Some code was written specifically for this project and is licensed
   under the MIT license.

Each file in this project should list the applicable copyright and
license at the top of the file.

Note that the terms of the LGPL require that the library as a whole is
also licensed under the LGPL (meaning the BSD- and MIT-licensed parts
are essentially dual-licensed).

Questions, bugs, enhancements and other feedback
------------------------------------------------
This library is far from complete, but should provide a useful base for
further expansion. If you have questions, problems or ideas for
improvement, feel free to create an isse or pullrequest in the github
repository (even for questions that are not really bugs):

[https://github.com/matthijskooijman/ArduinoUnix](https://github.com/matthijskooijman/ArduinoUnix)

Alternatively, you can drop by on IRC in the `#ArduinoUnix` channel on
the Freenode network with any feedback.
