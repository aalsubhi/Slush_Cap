#!/bin/bash -x

ifndef $(MSPGCCDIR)
	MSPGCCDIR=$(HOME)/ti/msp430-gcc
endif

#DEVICE = msp430fr5739
#DEVICE = msp430fr5969
#DEVICE = msp430fr6989
DEVICE = msp430fr5989
#paths
SUPPORT_FILE_DIRECTORY = $(MSPGCCDIR)/include

# compiler options
CC      = $(MSPGCCDIR)/bin/msp430-elf-g++

CXXFLAGS = -I . -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -g -Os -mhwmult=f5series -std=c++11
LFLAGS = -L . -L $(SUPPORT_FILE_DIRECTORY)

# mspdebug driver
DRIVER:= tilib


# Compiling
all: ADC.elf

%.elf : %.c
	$(CC) $(CXXFLAGS) $(LFLAGS) $(OBJ) -D __$(DEVICE)__ $< -o $@



# Upload to board
install: ADC.elf
		mspdebug $(DRIVER) "prog ADC.elf" --allow-fw-update


clean:
	rm -f  *.o *.elf
