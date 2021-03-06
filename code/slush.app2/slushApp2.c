
/* This App is written to evaluate the slush Cap project in term of continuous sensing.
This App for sensing light and wetness levels every 2 second using TimerA0 and send these info through radio.
This App runs on msp430fr5989.
This code is written by Arwa Alsubhi, October, 2018.
*/


#include <msp430fr5989.h>
#include <stdint.h>
#include "cc1101.h"


#define TX_BUFFER_SIZE 4
void Ports_config();
void PortsINT_config();
void TimerA_config();
void Radio_Pins_config();
void ADC_Pins_config();
void ADC_config();

uint8_t TX_buffer[TX_BUFFER_SIZE] = {0};
uint8_t i=0, flag=0;
uint16_t ADC_value[2]={0};
int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;
Ports_config(); //  setting all ports as input with pull down resistors
TimerA_config();
ADC_Pins_config();
ADC_config();
PJDIR |= BIT1; // set Radio Gate(PJ.1) as output
P5DIR |= BIT0; // set wetness sensor Gate P5.0 as output
P9DIR |=BIT0; // set light sensor Gate P9.0 as output

 for(;;)
 {
     __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/ interrupts
     if (  (P1IN & BIT0) && (P2IN & BIT7)) // if the   P2.7 wetness and P1.0 light sensors capacitors are charged
     {

        // Note that  ADC12CTL0 ,  ADC12CTL1 and ADC12MCTL0 Can be modified only when ADC12ENC = 0.
        ADC12MCTL0 |= ADC12INCH_10 ;       // ADC12INCH_1-> A10 ADC input select; Vref = VCC = 2V
      //turn on the wetness sensor by driving the gate high
          P5OUT |=BIT0;
        __delay_cycles(1000);
        // Enable and start a single conversion
         ADC12CTL0 |= ADC12SC| ADC12ENC;
        _bis_SR_register(LPM0_bits | GIE); // LPM0, ADC12_ISR will force exit
        ADC_value[0] = ADC12MEM0; // Save MEM0
        P5OUT &= ~BIT0; // turn off the weteness sensor by driving the gate low
       ADC12CTL0 &= ~ADC12ENC;                  //disable ADC conversion so we can modify ADC12MCTL0 register and deselect A10 and select A9
       ADC12MCTL0 &= ~ADC12INCH_10 ;       // deselct A10
       ADC12MCTL0 |= ADC12INCH_9 ;       //  select A9

      //turn on the light sensor by driving the gate high
      P9OUT |=BIT0;
      __delay_cycles(1000);
      // Enable and start a single conversion
       ADC12CTL0 |= ADC12SC | ADC12ENC;
      _bis_SR_register(LPM0_bits | GIE); // LPM0, ADC12_ISR will force exit
      ADC_value[1] = ADC12MEM0; // Save MEM0
      P9OUT &= ~BIT0; // turn off the light sensor by driving the gate low
      ADC12CTL0 &= ~ADC12ENC;                  //disable ADC conversion
      ADC12MCTL0 &=~ ADC12INCH_9 ;       // deselct A9
      
       // converting wetness and light sensors  from 32 bits variable to array of 8 bits to send it through radio

      TX_buffer[1] = (uint8_t) ((ADC_value[0] & 0x0000FF00) >> 8  );
      TX_buffer[0] = (uint8_t) ((ADC_value[0] & 0X000000FF)       );

      TX_buffer[3] = (uint8_t) ((ADC_value[1] & 0x0000FF00) >> 8  );
      TX_buffer[2] = (uint8_t) ((ADC_value[1] & 0X000000FF)       );

    if (P3IN & BIT3) // if the radio capacitor is fully charged
   {
      //turn on the radio by driving the gate high
       PJOUT |=BIT1;
      __delay_cycles(100000);
       //Radio startup
     Init();
     SetDataRate(4); // Needs to be the same in Tx and Rx
     SetLogicalChannel(1); // Needs to be the same in Tx and Rx
     SetTxPower(3); // set to 3 or 4
    __delay_cycles(100000);
	  SendDataNoWait(TX_buffer, TX_BUFFER_SIZE);
    __delay_cycles(100000);
	  //turn off the radio by driving the gate low
	  PJOUT &= ~BIT1;
    // configure all SPI pins of thr Radio as input with pull down resistor to save power
    Radio_Pins_config();
      }//end RadioCap status pin if
    }//end Wetness sensor and light sensor Caps status pin if
  }//end for loop
}//end main
////////////////////////////////////////////////////////////////////
 void Ports_config(){

   // setting all ports as input with pull down resistors

     P1DIR = 0; // 1- set p1 to input (0)
     P1REN = 0xFF; // 2- 0xff to enable pull up/down resistors for all 8 bits of p1
     P1OUT = 0; // 3- pull down enable (0)

     P2DIR = 0;
     P2REN = 0xFF;
     P2OUT = 0;

     P3DIR = 0;
     P3REN = 0xFF;
     P3OUT = 0;

     P4DIR = 0;
     P4REN = 0xFF;
     P4OUT = 0;

     P5DIR = 0;
     P5REN = 0xFF;
     P5OUT = 0;

     P6DIR = 0;
     P6REN = 0xFF;
     P6OUT = 0;

     P7DIR = 0;
     P7REN = 0xFF;
     P7OUT = 0;

     P8DIR = 0;
     P8REN = 0xFF;
     P8OUT = 0;

     P9DIR = 0;
     P9REN = 0xFF;
     P9OUT = 0;

     PJDIR = 0;
     PJREN = 0xFF;
     PJOUT = 0;

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
///////////////////////////////////////
void ADC_Pins_config(){
  // Configure P9.2 for ADC to read data from wetness sensor A10
    P9DIR &=~ BIT2; // set P9.2 as input
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;


    // Configure P9.1 for ADC to read data from light sensor A9
    P9DIR &=~ BIT1; // set  P9.1 as input
    P9SEL1 |= BIT1;
    P9SEL0 |= BIT1;


}
//////////////////////////////////////////////////////
void ADC_config(){

  // Note that  ADC12CTL0 ,  ADC12CTL1 and ADC12MCTL0 Can be modified only when ADC12ENC = 0.
  ADC12CTL0 &= ~ADC12ENC;                  //disable ADC conversion
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // ADC12SHT0_2-> sample-and-hold time=16 cycles, ADC12ON-> ADC enable
  ADC12CTL1 = ADC12SHP;                   // Source clock is sample timer
  ADC12CTL2 |= ADC12RES_2;                // ADC12RES_2->12-bit conversion results
  ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt


}
//////////////////////////////////////////////////////////
void Radio_Pins_config(){
  P3DIR &= ~BIT4 | ~ BIT5 | ~ BIT6;                    // P3.4, P3.5,P3.6 as input
  P3OUT |= BIT4 | BIT5 | BIT6;                      // Configure P3.4, P3.5 , P3.6 as pulled
  P3REN &= ~BIT4 | ~BIT5 | ~BIT6;                      // P3.4, P3.5, P3.6  pull-down register enable
}
////////////////////////////////////////////////////////////

// for Interrupt service routine, don't use #pragma, it does not work with GNU compiler (gcc). it works only with TI compiler.
// use  "__attribute__" instead
/////////////////////////////////////////////////////////////////////
//Interrupt service routine for Timer_A0 to wake up from LPM3 every 2 seconds
 void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A(void)
 {

         TA0CTL |= TACLR; // TACLR-> to clear TAR(Timer A Register), the clock divider logic (the divider setting remains unchanged), and the count direction
         TA0CTL &= ~TAIFG; // TAIFG-> Timer_A interrupt flag, 0b = No interrupt pending
         __bic_SR_register_on_exit(LPM3_bits|GIE); //  Exit LPM3 with enabling interrupt

 }
 ///////////////////////////////////////////////////////////////////
 //Interrupt service routine for ADC12
  void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
 {
 __bic_SR_register_on_exit(LPM0_bits|GIE); // Exit LPM0

 }
