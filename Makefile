#_______________________________________________________________________________
# Makefile to use with Arduino with Sanguino core on OSX, to use atmega644 and atmega1260 
#                        Based on edam's Arduino makefile
#
# Sanguino Makefile by Javier Montero is licensed under a 
# Creative Commons Attribution 3.0 Unported License.
# http://creativecommons.org/licenses/by/3.0/
#_______________________________________________________________________________
#
# Here is a complete list of configuration parameters:
#
# ARDUINODIR   The path where the Arduino software is installed on your system.
#
# ARDUINOCONST The Arduino software version, as an integer, used to define the
#              ARDUINO version constant. This defaults to 100 if undefined.
#
# AVRDUDECONF  The avrdude.conf to use. If undefined, this defaults to a guess
#              based on where the avrdude in use is. If empty, no avrdude.conf
#              is passed to avrdude (to the system default is used).
#
# AVRTOOLSPATH A space-separated list of directories to search in order when
#              looking for the avr build tools. This defaults to the system PATH
#              followed by subdirectories in ARDUINODIR if undefined.
#
# BOARD        Specify a target board type.  Run `make boards` to see available
#              board types.
#
# LIBRARIES    A list of Arduino libraries to build and include.  This is set
#              automatically if a .ino (or .pde) is found.
#
# SERIALDEV    The unix device name of the serial device that is the Arduino.
#              If unspecified, an attempt is made to determine the name of a
#              connected Arduino's serial device.
#
# SOURCES      A list of all source files of whatever language.  The language
#              type is determined by the file extension.  This is set
#              automatically if a .ino (or .pde) is found.
#
# TARGET       The name of the target file.  This is set automatically if a
#              .ino (or .pde) is found, but it is not necessary to set it
#              otherwise.
#
# This makefile also defines the following goals for use on the command line
# when you run make:
#
# all          This is the default if no goal is specified.  It builds the
#              target.
#
# target       Builds the target.
#
# upload       Uploads the last built target to an attached Arduino.
#
# clean        Deletes files created during the build.
#
# boards       Display a list of available board names, so that you can set the
#              BOARD environment variable appropriately.
#
# monitor      Start `screen` on the serial device.  This is meant to be an
#              equivalent to the Arduino serial monitor.
#
# size         Displays size information about the bulit target.
#
# <file>       Builds the specified file, either an object file or the target,
#              from those that that would be built for the project.
#_______________________________________________________________________________
#

#ARDUINODIR := /Applications/Arduino.app/Contents/Java

# default arduino software directory, check software exists
ifndef ARDUINODIR
ARDUINODIR := $(firstword $(wildcard ~/opt/arduino /usr/share/arduino))
endif
ifeq "$(wildcard $(ARDUINODIR)/hardware/sanguino/boards.txt)" ""
$(error ARDUINODIR is not set correctly; arduino software not found)
endif

# default arduino version
ARDUINOCONST ?= 100

# default path for avr tools
ifndef AVRTOOLSPATH
AVRTOOLSPATH := $(subst :, , $(PATH))
AVRTOOLSPATH += $(ARDUINODIR)/hardware/tools
AVRTOOLSPATH += $(ARDUINODIR)/hardware/tools/avr/bin
endif

# auto mode?
INOFILE := $(wildcard *.ino *.pde)
ifdef INOFILE
ifneq "$(words $(INOFILE))" "1"
$(error There is more than one .pde or .ino file in this directory!)
endif

# automatically determine sources and targeet
TARGET := $(basename $(INOFILE))
SOURCES := $(INOFILE) \
	$(wildcard *.c *.cc *.cpp) \
	$(wildcard $(addprefix util/, *.c *.cc *.cpp)) \
	$(wildcard $(addprefix utility/, *.c *.cc *.cpp))

# automatically determine included libraries
LIBRARIES := $(filter $(notdir $(wildcard $(ARDUINODIR)/libraries/*)), \
	$(shell sed -ne "s/^ *\# *include *[<\"]\(.*\)\.h[>\"]/\1/p" $(SOURCES)))

endif

# no serial device? make a poor attempt to detect an arduino
SERIALDEVGUESS := 0
ifeq "$(SERIALDEV)" ""
SERIALDEV := $(firstword $(wildcard \
	/dev/ttyACM? /dev/ttyUSB? /dev/tty.usbserial* /dev/tty.usbmodem*))
SERIALDEVGUESS := 1
endif

# software
findsoftware = $(firstword $(wildcard $(addsuffix /$(1), $(AVRTOOLSPATH))))
CC := $(call findsoftware,avr-gcc)
CXX := $(call findsoftware,avr-g++)
LD := $(call findsoftware,avr-ld)
AR := $(call findsoftware,avr-ar)
OBJCOPY := $(call findsoftware,avr-objcopy)
AVRDUDE := $(call findsoftware,avrdude)
AVRSIZE := $(call findsoftware,avr-size)

# files
TARGET := $(if $(TARGET),$(TARGET),a.out)
OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))
DEPFILES := $(patsubst %, .dep/%.dep, $(SOURCES))
ARDUINOCOREDIR := $(ARDUINODIR)/hardware/sanguino/cores/arduino
ARDUINOLIB := .lib/arduino.a
ARDUINOLIBLIBSDIR := $(ARDUINODIR)/libraries
ARDUINOLIBLIBSPATH := $(foreach lib, $(LIBRARIES), \
	$(ARDUINODIR)/libraries/$(lib)/ $(ARDUINODIR)/libraries/$(lib)/utility/ )
ARDUINOLIBOBJS := $(foreach dir, $(ARDUINOCOREDIR) $(ARDUINOLIBLIBSPATH), \
	$(patsubst %, .lib/%.o, $(wildcard $(addprefix $(dir)/, *.c *.cpp))))
ifeq "$(AVRDUDECONF)" ""
ifeq "$(AVRDUDE)" "$(ARDUINODIR)/hardware/tools/avr/bin/avrdude"
AVRDUDECONF := $(ARDUINODIR)/hardware/tools/avr/etc/avrdude.conf
else
AVRDUDECONF := $(wildcard $(AVRDUDE).conf)
endif
endif

# no board?
ifndef BOARD
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error BOARD is unset.  Type 'make boards' to see possible values)
endif
endif
endif

# obtain board parameters from the arduino boards.txt file
BOARDS_FILE := $(ARDUINODIR)/hardware/sanguino/boards.txt
BOARD_BUILD_MCU := \
	$(shell sed -ne "s/$(BOARD).build.mcu=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_BUILD_FCPU := \
	$(shell sed -ne "s/$(BOARD).build.f_cpu=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_BUILD_VARIANT := \
	$(shell sed -ne "s/$(BOARD).build.variant=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_UPLOAD_SPEED := \
	$(shell sed -ne "s/$(BOARD).upload.speed=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_UPLOAD_PROTOCOL := \
	$(shell sed -ne "s/$(BOARD).upload.protocol=\(.*\)/\1/p" $(BOARDS_FILE))

# invalid board?
ifeq "$(BOARD_BUILD_MCU)" ""
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error BOARD is invalid.  Type 'make boards' to see possible values)
endif
endif
endif

#git version
#GITVERSION := -D'gitversion="$(shell git describe)"'

# flags
CPPFLAGS := -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections
CPPFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CPPFLAGS += -mmcu=$(BOARD_BUILD_MCU)
CPPFLAGS += -DF_CPU=$(BOARD_BUILD_FCPU) -DARDUINO=$(ARDUINOCONST)
CPPFLAGS += -I. -Iutil -Iutility -I$(ARDUINOCOREDIR)
CPPFLAGS += -I$(ARDUINODIR)/hardware/sanguino/variants/$(BOARD_BUILD_VARIANT)/
CPPFLAGS += $(addprefix -I$(ARDUINODIR)/libraries/, $(LIBRARIES))
CPPFLAGS += $(patsubst %, -I$(ARDUINODIR)/libraries/%/utility, $(LIBRARIES))
CPPFLAGS += -Wno-write-strings
CPPFLAGS += -Wall
# Incluimos la variable GITVERSION 
#CPPFLAGS += $(GITVERSION) 
CPPDEPFLAGS = -MMD -MP -MF .dep/$<.dep
CPPINOFLAGS := -x c++ -include $(ARDUINOCOREDIR)/Arduino.h
AVRDUDEFLAGS := $(addprefix -C , $(AVRDUDECONF)) -D
AVRDUDEFLAGS += -V -v
AVRDUDEFLAGS += -p $(BOARD_BUILD_MCU) -P $(SERIALDEV) -F
AVRDUDEFLAGS += -c $(BOARD_UPLOAD_PROTOCOL) -b $(BOARD_UPLOAD_SPEED)
LINKFLAGS := -Os -Wl,--gc-sections -mmcu=$(BOARD_BUILD_MCU)

# figure out which arg to use with stty
STTYFARG := $(shell stty --help > /dev/null 2>&1 && echo -F || echo -f)

# include dependencies
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPFILES)
endif

# default rule
.DEFAULT_GOAL := all

#_______________________________________________________________________________
#                                                                          RULES

.PHONY:	all target upload clean boards monitor size

all: target

target: $(TARGET).hex

upload:
	@echo "\nUploading to board..."
	@test -n "$(SERIALDEV)" || { \
		echo "error: SERIALDEV could not be determined automatically." >&2; \
		exit 1; }
	@test 0 -eq $(SERIALDEVGUESS) || { \
		echo "*GUESSING* at serial device:" $(SERIALDEV); \
		echo; }
	stty $(STTYFARG) $(SERIALDEV) hupcl
	$(AVRDUDE) $(AVRDUDEFLAGS) -U flash:w:$(TARGET).hex:i

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET).elf $(TARGET).hex $(ARDUINOLIB) *~
	rm -rf .lib .dep

boards:
	@echo Available values for BOARD:
	@sed -ne '/^#/d; /^[^.]\+\.name=/p' $(BOARDS_FILE) | \
		sed -e 's/\([^.]\+\)\.name=\(.*\)/\1            \2/' \
			-e 's/\(.\{12\}\) *\(.*\)/\1 \2/'

monitor:
	@test -n "$(SERIALDEV)" || { \
		echo "error: SERIALDEV could not be determined automatically." >&2; \
		exit 1; }
	@test -n `which screen` || { \
		echo "error: can't find GNU screen, you might need to install it." >&2 \
		ecit 1; }
	@test 0 -eq $(SERIALDEVGUESS) || { \
		echo "*GUESSING* at serial device:" $(SERIALDEV); \
		echo; }
	screen $(SERIALDEV)

size: $(TARGET).elf
	echo && $(AVRSIZE) --format=avr --mcu=$(BOARD_BUILD_MCU) $(TARGET).elf

# building the target

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

.INTERMEDIATE: $(TARGET).elf

$(TARGET).elf: $(ARDUINOLIB) $(OBJECTS)
	$(CC) $(LINKFLAGS) $(OBJECTS) $(ARDUINOLIB) -o $@

%.o: %.c
	mkdir -p .dep/$(dir $<)
	$(COMPILE.c) $(CPPDEPFLAGS) -o $@ $<

%.o: %.cpp
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.cc
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.C
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.ino
	#mkdir -p .dep/$(dir $<)
	mkdir -p .dep/
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $(CPPINOFLAGS) $<

%.o: %.pde
	#mkdir -p .dep/$(dir $<)
	mkdir -p .dep/
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ -x c++ -include $(ARDUINOCOREDIR)/Arduino.h $<



# building the arduino library

$(ARDUINOLIB): $(ARDUINOLIBOBJS)
	$(AR) rcs $@ $?

.lib/%.c.o: %.c
	mkdir -p $(dir $@)
	$(COMPILE.c) -o $@ $<

.lib/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

.lib/%.cc.o: %.cc
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

.lib/%.C.o: %.C
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

