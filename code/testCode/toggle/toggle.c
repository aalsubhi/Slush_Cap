#include <msp430fr5989.h>
#include <stdint.h>
#include<stdlib.h>

//void Ports_config();
int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Clear the LOCKLPM5 bit in the PM5CTL0 register. It is important to unlock the state of the I/Os and the modules

P1DIR |= BIT0;
P1DIR |= BIT1;
P1DIR |= BIT2;
P1DIR |= BIT3;
P1DIR |= BIT4;
P1DIR |= BIT5;
P1DIR |= BIT6;
P1DIR |= BIT7;

P2DIR |= BIT0;
P2DIR |= BIT1;
P2DIR |= BIT2;
P2DIR |= BIT3;
P2DIR |= BIT4;
P2DIR |= BIT5;
P2DIR |= BIT6;
P2DIR |= BIT7;

P3DIR |= BIT0;
P3DIR |= BIT1;
P3DIR |= BIT2;
P3DIR |= BIT3;
P3DIR |= BIT4;
P3DIR |= BIT5;
P3DIR |= BIT6;
P3DIR |= BIT7;

P4DIR |= BIT0;
P4DIR |= BIT1;
P4DIR |= BIT2;
P4DIR |= BIT3;
// P4DIR |= BIT4;
// P4DIR |= BIT5;
// P4DIR |= BIT6;
// P4DIR |= BIT7;


P5DIR |= BIT0;
// P5DIR |= BIT2;
P5DIR |= BIT3;
// P5DIR |= BIT4;
// P5DIR |= BIT5;
// P5DIR |= BIT6;
// P5DIR |= BIT7;

// P6DIR |= BIT0;
// P6DIR |= BIT2;
// P6DIR |= BIT3;
// P6DIR |= BIT4;
// P6DIR |= BIT5;
// P6DIR |= BIT6;
// P6DIR |= BIT7;
//
// P7DIR |= BIT0;
// P7DIR |= BIT2;
// P7DIR |= BIT3;
// P75DIR |= BIT4;
// P7DIR |= BIT5;
// P7DIR |= BIT6;
// P7DIR |= BIT7;
//
//
// P8DIR |= BIT0;
// P8DIR |= BIT2;
// P8DIR |= BIT3;
// P8DIR |= BIT4;
// P8DIR |= BIT5;
// P8DIR |= BIT6;
// P8DIR |= BIT7;

P9DIR |= BIT0;
P9DIR |= BIT1;
P9DIR |= BIT2;
P9DIR |= BIT3;
P9DIR |= BIT4;
P9DIR |= BIT5;
P9DIR |= BIT6;
P9DIR |= BIT7;

//PJDIR |= BIT0;
PJDIR |= BIT1;
PJDIR |= BIT2;
//PJDIR |= BIT3;
// PJDIR |= BIT4;
// PJDIR |= BIT5;
// PJDIR |= BIT6;
// PJDIR |= BIT7;

for (;;){
  // P1OUT ^= BIT0;
  // P1OUT ^= BIT1;
  // P1OUT ^= BIT2;
  // P1OUT ^= BIT3;
  // P1OUT ^= BIT4;
  // P1OUT ^= BIT5;
  // P1OUT ^= BIT6;
  // P1OUT ^= BIT7;
  //
  // P2OUT ^= BIT0;
  // P2OUT ^= BIT1;
  // P2OUT ^= BIT2;
  // P2OUT ^= BIT3;
  // P2OUT ^= BIT4;
  // P2OUT ^= BIT5;
  // P2OUT ^= BIT6;
  // P2OUT ^= BIT7;
  //
  // P3OUT ^= BIT0;
  // P3OUT ^= BIT1;
  // P3OUT ^= BIT2;
  // P3OUT ^= BIT3;
  // P3OUT ^= BIT4;
  // P3OUT ^= BIT5;
  // P3OUT ^= BIT6;
  // P3OUT ^= BIT7;
  //
  // P4OUT ^= BIT0;
  // P4OUT ^= BIT1;
  // P4OUT ^= BIT2;
  // P4OUT ^= BIT3;
  //
  // P5OUT ^= BIT0;
  // P5OUT ^= BIT3;

  P9OUT ^= BIT0;
  // P9OUT ^= BIT1;
  // P9OUT ^= BIT2;
  // P9OUT ^= BIT3;
  // P9OUT ^= BIT4;
  // P9OUT ^= BIT5;
  // P9OUT ^= BIT6;
  // P9OUT ^= BIT7;


  // PJOUT ^= BIT1;
  // PJOUT ^= BIT2;
 __delay_cycles(1000000); // 100ms delay

}




}
