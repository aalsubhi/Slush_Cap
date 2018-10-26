#include <msp430fr6989.h>
#include <stdint.h>
#include "cc1101.h"

int32_t temp=0;
int32_t ADC_value=0 ;
#define TX_BUFFER_SIZE 4
uint8_t TX_buffer [TX_BUFFER_SIZE] = {0},i;

void TimerA_config();
void ADC_Pins_config();
void ADC_config();



int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;
//TimerA_config();
ADC_Pins_config();
ADC_config();
P1DIR |= BIT0; // set p1.0 as output
for(i=0;i<TX_BUFFER_SIZE;i++)
{
TX_buffer [i] = 1;
}


 for(;;)
 {
       //__bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/ interrupts
      // put delay to let tempreture sensor settle
    // ADC12CTL0 |= ADC12ENC | ADC12SC; // Sampling and conversion start
  //  __bis_SR_register(LPM0_bits + GIE); // LPM0, ADC12_ISR will force exit
    //   __no_operation();                   // For debugger
    // Poll busy bit waiting for conversion to complete
    // while (ADC12CTL1 & ADC12BUSY) {
    // }

    //ADC_value = ADC12MEM0; // Save MEM0
     //temp = ADC_value *0.488; // Adc * 2000(mv)/4096,  2000mv becuse the Flicker's mcu runs at 2V, 12 bits Adc = 2^12 = 4096
     // temp=0;
     // temp = (((ADC_value *3300)/4096)-500)/10;
     // converting tempreture from 32 bits variable to array of 8 bits
      // TX_buffer[3] = (uint8_t) ((ADC_value & 0xFF000000) >> 24 );
      // TX_buffer[2] = (uint8_t) ((ADC_value & 0x00FF0000) >> 16 );
      // TX_buffer[1] = (uint8_t) ((ADC_value & 0x0000FF00) >> 8  );
      // TX_buffer[0] = (uint8_t) ((ADC_value & 0X000000FF)       );
     //delay(100);
    //Radio startup
    Init();
   SetDataRate(4); // Needs to be the same in Tx and Rx
   SetLogicalChannel(1); // Needs to be the same in Tx and Rx
   SetTxPower(3);
   delay(1000);
  SendDataNoWait(TX_buffer, TX_BUFFER_SIZE);
  __delay_cycles(100000);
  P1OUT |=BIT0; //set high
   __delay_cycles(100000);
   P1OUT &= ~BIT0;
   __delay_cycles(1000000);
   P1OUT |=BIT0; //set high
   __delay_cycles(1000000);
    P1OUT &= ~BIT0;
    __delay_cycles(1000000);
 }//end for loop


}//end main



void ADC_Pins_config(){
  // Configure P1.3 for ADC to read data from tempreture sensor
  P1SEL1 |= BIT3;
  P1SEL0 |= BIT3;
}
////////////////////////////////////
void ADC_config(){

  // Configure ADC12
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // ADC12SHT0_2-> sample-and-hold time=16 cycles, ADC12ON-> ADC enable
  ADC12CTL1 = ADC12SHP;                   // Source clock is sample timer
  ADC12CTL2 |= ADC12RES_2;                // ADC12RES_2->12-bit conversion results
  ADC12MCTL0 |= ADC12INCH_3;        // ADC12INCH_2-> A3 ADC input select;  Vref =VCC
 //ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt
}
//////////////////////////////////////
void TimerA_config(){
  // configure ACLK by using Clock System Control Registers (CSCTLx)
  CSCTL0 = CSKEY; // Unlock clock registers
  CSCTL2 = SELA__VLOCLK; // ACLK Source Select VLOCLK (10-kHz typical frequency)
  CSCTL3 |= DIVA__8; // ACLK Source Divider f(ACLK)/8 = 10kHz/8 = 1.25kHz/s = 1250 Hz/s
  CSCTL4 &= ~VLOOFF; // 0b = VLO(Internal Very-Low-Power Low-Frequency Oscillator) is on // VLO=10-kHz
  CSCTL0_H = 0x00;  // Lock clock registers// Write access is disabled by writing an incorrect password in byte mode to the CSCTL0 upper byte (CSCTL0_H).

  // Configure TimerA0 and its interrupt
  TA0CTL = TASSEL_1 + MC__UPDOWN + ID__2; //TA0CTL-> Timer A0 Control Registers, TASSEL_1-> Timer A clock source select 1 =ACLK, MC__UPDOWN-> Timer counts up to TAxCCR0 then down to 0,
  //  ID__2 -> clock divider by 2, 1250 Hz/2= 625 Hz/s
  TA0CCR0 = 1150 ; //TA0CCR0 (Timer A0 Capture Compare Register 0)-> 16-bit counter should not exced 65536 = 2^16 , since the clock is 625 Hz/s and the timer count mode is up/down 1250 gives 2 second (by counting up to 625 and down to 0)
  // for some reson I don't understand(I think becuse VLOCLK is not Very accurate ) , 1250 gives more than 2 second. so, I tried 1150 and it gave 2 second. you can test that by toggling a pin in the loop and use the scope to watch that pin.
  // if we need one second we set up the count mode to up and use 625 for TA0CCR0 value
  TA0CCTL0 = CCIE;          //set TA0CCTL0->Timer_A0 Capture/Compare Control 0 Register to CCIE-> Capture/compare interrupt enable
  TA0CTL |=TAIE; // TAIE-> Timer_A interrupt enable. This bit enables the TAIFG interrupt request, 0b = Interrupt disabled, 1b = Interrupt enabled

}
////////////////////////////////////////
//  void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
// {
// __bic_SR_register_on_exit(LPM0_bits); // Exit CPU
//
// }
//////////////////////////////////////////
//Interrupt service routine for Timer_A0
 // void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A(void)
 // {
 //
 //         TA0CTL |= TACLR; // TACLR-> to clear TAR(Timer A Register), the clock divider logic (the divider setting remains unchanged), and the count direction
 //         TA0CTL &= ~TAIFG; // TAIFG-> Timer_A interrupt flag, 0b = No interrupt pending
 //         __bic_SR_register_on_exit(LPM3_bits | GIE); //  Exit LPM3 with enabling interrupt
 //
 // }
