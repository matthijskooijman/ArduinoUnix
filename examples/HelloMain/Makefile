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
# This Makefile shows three different ways of linking against
# ArduinoUnix:
#  - Include all sources in a single big compilation (compile)
#  - Compile .o files for ArduinoUnix and link against them (link-objects)
#  - Compile a .a file for ArduinoUnix and link against that (link-liba)

# Include the ArduinoUnix makefile
ARDUINO_UNIX_PATH := ../..
include $(ARDUINO_UNIX_PATH)/Makefile.mk

SOURCE    := hello.cpp
OBJECT    := $(SOURCE:.cpp=.o)
BINARY    := binary
CXXFLAGS  := -I$(ARDUINO_UNIX_INCLUDE_PATH) -Wall
LDFLAGS   := -lstdc++

# Default to a all-in-one compile
all: compile

compile: $(SOURCE) $(ARDUINO_UNIX_SOURCES)
	gcc $(CXXFLAGS) $(LDFLAGS) $(SOURCE) $(ARDUINO_UNIX_SOURCES) -o $(BINARY)

%.o: %.cpp
	gcc -c $(CXXFLAGS) $< -o $@

link-objects: $(OBJECT) $(ARDUINO_UNIX_OBJECTS)
	gcc $(LDFLAGS) $(OBJECT) $(ARDUINO_UNIX_OBJECTS) -o $(BINARY)

link-liba: $(OBJECT) $(ARDUINO_UNIX_LIB_A)
	gcc $(LDFLAGS) $(OBJECT) $(ARDUINO_UNIX_LIB_A) -o $(BINARY)

clean:
	rm -f $(OBJECT) $(BINARY)
