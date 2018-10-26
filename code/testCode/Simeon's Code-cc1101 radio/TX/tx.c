/*
 * Send IV-curves (raw IV points really) over-the-air as fast as possible to anyone listeneing
 */
#include <msp430fr5969.h>
#include "cc1101.h"

#define TX_BUFFER_SIZE 5
#define RX_BUFFER_SIZE 5

uint8_t tx_buffer[TX_BUFFER_SIZE]={0};
uint8_t rx_buffer[RX_BUFFER_SIZE]={0};
uint8_t loop_index;

void led_init(){
	P1DIR |= BIT0;
}

void led_on(){
	P1OUT |= BIT0;
}

void led_off(){
	P1OUT &= ~BIT0;
}

void led_toggle(){
	P1OUT ^= BIT0;
}

int main(void){
	uint16_t temp = 0;

	//Stop watchdog timer
	WDTCTL = WDTPW + WDTHOLD;
  //Free or unlock pins from high Impedance
  PM5CTL0 &= ~LOCKLPM5;

	led_init();

	// Init results
	tx_buffer[0] = DEVICE_ID;

	// Radio startup
	Init();
	SetDataRate(4); // Needs to be the same in Tx and Rx
	SetLogicalChannel(1); // Needs to be the same in Tx and Rx
	SetTxPower(0);
	Sleep();

	// Main loop
	while(1) {
		led_toggle();
    tx_buffer[1] = 5;
    tx_buffer[2] = 10;

  	// Turn on radio, for speed, don't sleep / wakeup
		Wakeup();
		__delay_cycles(100);
		SendData(tx_buffer, TX_BUFFER_SIZE);
		Sleep();

    __delay_cycles(100000);
	}

	return 1;
}
