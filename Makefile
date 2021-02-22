# Those that specify a NO_ARDUINO environment variable will
# be able to use this Makefile with no Arduino dependency.
#NO_ARDUINO

# The teensy version to use, LC, 30, 31, 32, 35, 36, 40, or 41
# or make command line `make TEENSY=32`
TEENSY = 32

# directory to build in
BUILDDIR = build

# directory to output binary
BINDIR = bin

# The name of your project (used to name the compiled .hex file)
TARGET = sidewinder

# options needed by many Arduino libraries to configure for Teensy 3.x/4.x
DEFINES = -DARDUINO=10813 -DTEENSYDUINO=153

# configurable options
DEFINES += -DUSB_SERIAL_SIDEWINDER_HID -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE -DDEBUG
#
# USB Type configuration:
#   -DUSB_SERIAL
#   -DUSB_DUAL_SERIAL
#   -DUSB_TRIPLE_SERIAL
#   -DUSB_KEYBOARDONLY
#   -DUSB_TOUCHSCREEN
#   -DUSB_HID_TOUCHSCREEN
#   -DUSB_HID
#   -DUSB_SERIAL_HID
#   -DUSB_MIDI
#   -DUSB_MIDI4
#   -DUSB_MIDI16
#   -DUSB_MIDI_SERIAL
#   -DUSB_MIDI4_SERIAL
#   -DUSB_MIDI16_SERIAL
#   -DUSB_AUDIO
#   -DUSB_MIDI_AUDIO_SERIAL
#   -DUSB_MIDI16_AUDIO_SERIAL
#   -DUSB_MTPDISK
#   -DUSB_RAWHID
#   -DUSB_FLIGHTSIM
#   -DUSB_FLIGHTSIM_JOYSTICK


# Other Makefiles and project templates for Teensy
#
# https://forum.pjrc.com/threads/57251?p=213332&viewfull=1#post213332
# https://github.com/apmorton/teensy-template
# https://github.com/xxxajk/Arduino_Makefile_master
# https://github.com/JonHylands/uCee


#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

ifndef NO_ARDUINO

# path location for Arduino libraries
LIBRARYPATH = libraries

ifeq ($(OS),Windows_NT)
	# path location for Teensy Loader, teensy_post_compile.exe, and teensy_reboot.exe (on Windows)
	TOOLSPATH = ${ProgramFiles(x86)}/Arduino/hardware/tools

	# path location for the arm-none-eabi compiler
	COMPILERPATH = $(TOOLSPATH)/arm/bin

	POSTCOMPILE = "$(TOOLSPATH)/teensy_post_compile.exe"

	REBOOT = "$(TOOLSPATH)/teensy_reboot.exe"
else
	# path location for Teensy Loader, teensy_post_compile, and teensy_reboot (on Linux)
	TOOLSPATH = $(abspath tools)

	# path location for the arm-none-eabi compiler
	COMPILERPATH = $(TOOLSPATH)/arm/bin

	POSTCOMPILE = $(TOOLSPATH)/teensy_post_compile

	REBOOT = $(TOOLSPATH)/teensy_reboot
endif

else
# Default to the normal GNU/Linux compiler path if NO_ARDUINO
COMPILERPATH ?= /usr/bin

endif

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************
CPUOPTIONS = -mthumb

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -Os -MMD 

# compiler options for C++ only
CXXFLAGS = -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -Wl,--gc-sections

# additional libraries to link
LIBS = -lm

# compiler options specific to teensy version
ifeq ($(TEENSY), 30)
	COREPATH = cores/teensy3
	CPUOPTIONS += -mcpu=cortex-m4
	MCU = MK20DX128
	MCU_LD = mk20dx128.ld
	CORE_SPEED ?= 48000000
	LDFLAGS += -Wl,--defsym=__rtc_localtime=0 --specs=nano.specs
else ifeq ($(TEENSY),$(filter $(TEENSY),31 32))
	COREPATH = cores/teensy3
	CPUOPTIONS += -mcpu=cortex-m4
	MCU = MK20DX256
	MCU_LD = mk20dx256.ld
	CORE_SPEED ?= 72000000
	LDFLAGS += -Wl,--defsym=__rtc_localtime=0 --specs=nano.specs
else ifeq ($(TEENSY), LC)
	COREPATH = cores/teensy3
	CPUOPTIONS += -mcpu=cortex-m0plus
	MCU = MKL26Z64
	MCU_LD = mkl26z64.ld
	CORE_SPEED ?= 48000000
	LDFLAGS += -Wl,--defsym=__rtc_localtime=0 --specs=nano.specs
	LIBS += -larm_cortexM0l_math
else ifeq ($(TEENSY), 35)
	COREPATH = cores/teensy3
	CPUOPTIONS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
	MCU = MK64FX512
	MCU_LD = mk64fx512.ld
	CORE_SPEED ?= 120000000
	LDFLAGS += -Wl,--defsym=__rtc_localtime=0 --specs=nano.specs
	LIBS += -larm_cortexM4lf_math
else ifeq ($(TEENSY), 36)
	COREPATH = cores/teensy3
	CPUOPTIONS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
	MCU = MK66FX1M0
	MCU_LD = mk66fx1m0.ld
	CORE_SPEED ?= 180000000
	LDFLAGS += -Wl,--defsym=__rtc_localtime=0 --specs=nano.specs
	LIBS += -larm_cortexM4lf_math
else ifeq ($(TEENSY), 40)
	COREPATH = cores/teensy4
	CPUOPTIONS += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
	CXXFLAGS += -fpermissive -Wno-error=narrowing
	MCU = IMXRT1062
	MCU_LD = imxrt1062.ld
	CORE_SPEED ?= 600000000
	LDFLAGS += -Wl,--relax
	LIBS += -larm_cortexM7lfsp_math -lstdc++
	DEFINES += -DARDUINO_TEENSY40
else ifeq ($(TEENSY), 41)
	COREPATH = cores/teensy4
	CPUOPTIONS += -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
	CXXFLAGS += -fpermissive -Wno-error=narrowing
	MCU = IMXRT1062
	MCU_LD = imxrt1062_t41.ld
	CORE_SPEED ?= 600000000
	LDFLAGS += -Wl,--relax
	LIBS += -larm_cortexM7lfsp_math -lstdc++
	DEFINES += -DARDUINO_TEENSY41
else
	$(error Invalid setting for TEENSY)
endif

CPPFLAGS += $(CPUOPTIONS) -D__$(MCU)__ -DF_CPU=$(CORE_SPEED) $(DEFINES) -Isrc -I$(COREPATH)
LDFLAGS += $(CPUOPTIONS) -T$(COREPATH)/$(MCU_LD)


# names for the compiler programs
ifeq ($(OS),Windows_NT)
	CC = "$(COMPILERPATH)/arm-none-eabi-gcc.exe"
	CXX = "$(COMPILERPATH)/arm-none-eabi-g++.exe"
	OBJCOPY = "$(COMPILERPATH)/arm-none-eabi-objcopy.exe"
	SIZE = "$(COMPILERPATH)/arm-none-eabi-size.exe"
else
	CC = $(COMPILERPATH)/arm-none-eabi-gcc
	CXX = $(COMPILERPATH)/arm-none-eabi-g++
	OBJCOPY = $(COMPILERPATH)/arm-none-eabi-objcopy
	SIZE = $(COMPILERPATH)/arm-none-eabi-size
endif

# automatically create lists of the sources and objects
LC_FILES := $(wildcard $(LIBRARYPATH)/*/*.c)
LCPP_FILES := $(wildcard $(LIBRARYPATH)/*/*.cpp)
TC_FILES := $(wildcard $(COREPATH)/*.c)
TCPP_FILES := $(filter-out $(COREPATH)/main.cpp, $(wildcard $(COREPATH)/*.cpp)) # ignore main.cpp within teensy cores
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp)
#INO_FILES := $(wildcard src/*.ino)

# include paths for libraries
L_INC := $(foreach lib,$(filter %/, $(wildcard $(LIBRARYPATH)/*/)), -I$(lib))

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o) #$(INO_FILES:.ino=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

# the actual makefile rules
all: $(BINDIR)/$(TARGET).hex

post_compile: $(BINDIR)/$(TARGET).hex
	@$(POSTCOMPILE) -file="$(notdir $(basename $<))" -path="$(BINDIR)" -tools="$(TOOLSPATH)"

reboot:
	@-$(REBOOT)

upload: post_compile reboot

$(BUILDDIR)/%.o: %.c
	@echo "[CC]  $<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo "[CXX] $<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -c "$<"

$(BINDIR)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo "[LD]  $@"
	@mkdir -p "$(dir $@)"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	@echo "[HEX] $@"
	@mkdir -p "$(dir $@)"
	@$(SIZE) $<
	@$(OBJCOPY) -O ihex -R .eeprom $< $@

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BINDIR)"
	@rm -rf "$(BUILDDIR)"
