#include <msp430fr5969.h>
#include <stdint.h>


volatile uint8_t TempCap = 0;
int32_t temp;
 int32_t ADC_value=0;



void TimerA_config();
void ADC_Pins_config();
void ADC_config();

int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;

ADC_Pins_config();
ADC_config();







 for(;;)
 {

       __delay_cycles(1000);
      // put delay to let tempreture sensor settle
      ADC12CTL0 |= ADC12ENC | ADC12SC; // Sampling and conversion start


    // __bis_SR_register(LPM0_bits + GIE); // LPM0, ADC12_ISR will force exit
    //   __no_operation();                   // For debugger
    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY) {
    }
     ADC_value = ADC12MEM0; // Save MEM0
     temp = (((ADC_value *3300L)/4096L)-500L)/10L;


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
  ADC12MCTL0 |= ADC12INCH_3 ;        // ADC12INCH_3-> A3 ADC input select; Vref =Vcc = 3.3
  //ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt

}
//////////////////////////////////////
