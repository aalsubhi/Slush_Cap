#include <msp430fr5989.h>
#include <stdint.h>


volatile uint8_t TempCap = 0;
int32_t temp=0;
int32_t ADC_value=0;
void Ports_config();

void PortsINT_config();
void TimerA_config();
void ADC_Pins_config();
void ADC_config();

int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;
Ports_config(); //  setting all ports as input with pull down resistors
//PortsINT_config(); //configure the inturreptable pins
//TimerA_config();
ADC_Pins_config();
ADC_config();
 //PJDIR |= BIT1; // set P3_Gate PJ.1 as output
 P5DIR |= BIT0; // set P2_Gate P5.0 as output






 for(;;)
 {
   int i=0;

   //__bis_SR_register(LPM3_bits + GIE); // Enter LPM3 w/ interrupts

   // if (TempCap) // if the tempreture sensor capacitor is charged
   // {
     //turn on the tempreture sensor by driving the gate high
     if (P2IN & BIT7) // if the tempreture sensor capacitor is charged
     {
       P5OUT |=BIT0;
      //PJOUT |=BIT1;
      __delay_cycles(1000);
      // put delay to let tempreture sensor settle
      ADC12CTL0 |= ADC12ENC | ADC12SC; // Sampling and conversion start
    // __bis_SR_register(LPM0_bits + GIE); // LPM0, ADC12_ISR will force exit
    //   __no_operation();                   // For debugger
    // Poll busy bit waiting for conversion to complete
    while ( ADC12BUSY) {
      i++;
    }
     ADC_value = ADC12MEM0; // Save MEM0
     //temp = ADC_value *0.488; // Adc * 2000(mv)/4096,  2000mv becuse the Flicker's mcu runs at 2V, 12 bits Adc = 2^12 = 4096
  //  temp = (((ADC_value *2000L)/4096L)-500L)/10L;
  // temp = ADC_value *2000L;
  // temp/=4096L;
  // temp -= 500L;
  // temp /=10L;
  //  TempCap =0;
    }

  //  else{
      //turn off the tempreture sensor by driving the gate low
  	  P5OUT &= ~BIT0;

  //  }

 }//end for loop


}//end main

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


///////////////////////////////////////////////////////////
void PortsINT_config(){
//set up interrupt for peripheral 3
  // P3DIR &= ~BIT3;                    // P3.3 as input
  // P3OUT |= BIT3;                      // Configure P3.3 as pulled
  // P3REN &= ~BIT3;                      // P3.3  pull-down register enable
  // P3IES &= ~BIT3;                  // P3IES-> Port 3 Interrupt Edge Select Register , lo/hi
  // P3IFG &= ~BIT3;               // cleared P3IFG-> Port 3 Interrupt Flag Register
  // P3IE |= BIT3;                    //P4IE-> Port 3 Interrupt Enable Register,  P3.3 interrupt enabled

//set up interrupt for peripheral 2
  P2DIR &= ~BIT7;                    // P2.7 as input
  P2OUT |= BIT7;                      // Configure P2.7 as pulled
  P2REN &= ~BIT7;                      // P2.7  pull-down register enable
  P2IES &= ~BIT7;                  // P2IES-> Port 2 Interrupt Edge Select Register , lo/hi
  P2IFG &= ~BIT7;               // cleared P2IFG-> Port 7 Interrupt Flag Register
  P2IE |= BIT7;


}
void ADC_Pins_config(){
  // Configure P1.1 for ADC to read data from tempreture sensor
  P1SEL1 |= BIT1;
  P1SEL0 |= BIT1;
}
////////////////////////////////////
void ADC_config(){

  // Configure ADC12
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // ADC12SHT0_2-> sample-and-hold time=16 cycles, ADC12ON-> ADC enable
  ADC12CTL1 = ADC12SHP;                   // Source clock is sample timer
  ADC12CTL2 |= ADC12RES_2;                // ADC12RES_2->12-bit conversion results
  ADC12MCTL0 |= ADC12INCH_1 ;       // ADC12INCH_1-> A1 ADC input select; Vref = VCC = 2V
  //ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt

}
//////////////////////////////////////
void TimerA_config(){
  // configure ACLK by using Clock System Control Registers (CSCTLx)
  CSCTL0 = CSKEY; // Unlock clock registers
  CSCTL2 = SELA__VLOCLK; // ACLK Source Select VLOCLK
  CSCTL3 |= DIVA__8; // ACLK Source Divider f(ACLK)/8
  CSCTL4 &= ~VLOOFF; // 0b = VLO(Internal Very-Low-Power Low-Frequency Oscillator) is on // VLO=10-kHz
  CSCTL0_H = 0x00;  // Lock clock registers// Write access is disabled by writing an incorrect password in byte mode to the CSCTL0 upper byte (CSCTL0_H).

  // Configure TimerA0 and its interrupt
  TA0CTL = TASSEL_1 + MC__UP + ID__2; //TA0CTL-> Timer A0 Control Registers, TASSEL_1-> Timer A clock source select 1 =ACLK, MC__UP-> operation mode "Up mode",  ID__2 -> clock divider by 2
  //TA0CCR0 = 105 ;  // this will fire the interrupt every 190 ms
  TA0CCR0 = 200 ;
  TA0CCTL0 = CCIE;          //set TA0CCTL0->Timer_A0 Capture/Compare Control 0 Register to CCIE-> Capture/compare interrupt enable
  TA0CTL |=TAIE; // TAIE-> Timer_A interrupt enable. This bit enables the TAIFG interrupt request, 0b = Interrupt disabled, 1b = Interrupt enabled

}
//////////////////////////////////////////////////////////




 // void __attribute__ ((interrupt(PORT3_VECTOR))) Port_3(void)
 // {
 //   P3IFG &=~BIT3;                        // P3.3 IFG cleared
 //   RadioCap= 1;
 //  // __bic_SR_register_on_exit(LPM3_bits);
 //
 // }

 // void __attribute__ ((interrupt(PORT2_VECTOR))) Port_3(void)
 // {
 //   P2IFG &=~BIT7;                        // P2.7 IFG cleared
 //   TempCap=1;
 //  // __bic_SR_register_on_exit(LPM3_bits);
 //
 // }

 // void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A(void) // don't use #pragma, it does not work with msbdebug
 // {
 //
 //
 //         TA0CTL |= TACLR; // TACLR-> to clear TAR(Timer A Register), the clock divider logic (the divider setting remains unchanged), and the count direction
 //         TA0CTL &= ~TAIFG; // TAIFG-> Timer_A interrupt flag, 0b = No interrupt pending
 //           __bic_SR_register_on_exit(LPM3_bits);
 //
 // }
//  void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
// {
// __bic_SR_register_on_exit(LPM0_bits); // Exit CPU
//
// }
