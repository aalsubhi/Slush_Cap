DEVICE = msp430fr5969

all:
	msp430-elf-gcc -g -O2 -I /Users/arwaalsubhi/ti/msp430_gcc/include -L /Users/arwaalsubhi/ti/msp430_gcc/include -mmcu=${DEVICE} ${input} -o ${output}

run:
	 mspdebug tilib "prog ${input}" --allow-fw-update
