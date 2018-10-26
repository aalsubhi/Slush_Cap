SUPPORT_FILE_DIRECTORY = /Users/arwaalsubhi/ti/msp430_gcc/include

DDEVICE_ID = 1

DEVICE  = msp430fr5969
CC      = msp430-elf-g++
GDB     = msp430-elf-gdb

CSTD_FLAGS = -std=c++11 -funsigned-char
DEBUG_FLAGS = -g -g3 -ggdb -gdwarf-2
ERROR_FLAGS = -Wall -Wextra -Wshadow -Wfatal-errors
NO_ERROR_FLAGS = -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable -Wno-type-limits -Wno-comment
MSP430_FLAGS = -mmcu=$(DEVICE) -mhwmult=f5series
REDUCE_SIZE_FLAGS = -O0

CXXFLAGS = -I $(SUPPORT_FILE_DIRECTORY) $(CSTD_FLAGS) $(DEBUG_FLAGS) $(ERROR_FLAGS) $(NO_ERROR_FLAGS) $(MSP430_FLAGS) $(REDUCE_SIZE_FLAGS)
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY)

OBJ = cc1101.o

all: $(OBJ) rx.c
	$(CC) $(CXXFLAGS) -DDEVICE_ID=$(DDEVICE_ID) $(LFLAGS) $< rx.c -o rx.elf

cc1101.o : cc1101.c
	$(CC) -c $(CXXFLAGS) $(LFLAGS) $< -o $@


install: all
	mspdebug tilib "prog rx.elf" --allow-fw-update

debug: all
	mspdebug tilib gdb &

gdb:
	$(GDB) -x cmd.gdb

clean:
	rm -f *.elf *.asm *.o
