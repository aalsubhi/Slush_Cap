DEVICE = msp430fr5989

all: cc1101.o slush.c
	msp430-elf-gcc -g -O2 -I /Users/arwaalsubhi/ti/msp430_gcc/include -L /Users/arwaalsubhi/ti/msp430_gcc/include -mmcu=${DEVICE} $< slush.c -o slush.elf

cc1101.o : cc1101.c
	msp430-elf-gcc -c -I /Users/arwaalsubhi/ti/msp430_gcc/include -L /Users/arwaalsubhi/ti/msp430_gcc/include  $< -o $@
run:
	 mspdebug tilib "prog slush.elf" --allow-fw-update
