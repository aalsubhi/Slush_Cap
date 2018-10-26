#include <msp430fr5969.h>
#include "cc1101.h"

#define sizetx 61
// #define GREENLED BIT0                   // LED

uint8_t RX_buffer[sizetx]={0};

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

int main(){
  uint8_t rx_size;

  WDTCTL = WDTPW | WDTHOLD;             //stop the watchdog timer
  PM5CTL0 &= ~LOCKLPM5;                 //Free or unlock pins from high Impedance

  led_init();

  //initialize CC1101 Radio
  Init();
  SetDataRate(4); // Needs to be the same in Tx and Rx
  SetLogicalChannel(1); // Needs to be the same as receiver
  RxOn();

  //Continuous loop
  for(;;){
    //Check if we receive something
    if(CheckReceiveFlag()){
      rx_size = ReceiveData(RX_buffer);
      if(RX_buffer[1] == 5){
        led_on();
        __delay_cycles(100000);
        led_off();
        __delay_cycles(100000);
      }
      RxOn();
    }
  }
}
