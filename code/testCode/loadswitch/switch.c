#include <msp430fr5969.h>



#define delay(x) __delay_cycles(x * 8000UL)
void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;

 P4OUT |= BIT2;
 P4DIR |= BIT2; // set Gate P4.2 as output

 for(;;)
 {

  // P4OUT &= ~BIT2;                  // set Gate low
  // delay(10);
  P4OUT |= BIT2;
  //  delay(10);
 }
 }
