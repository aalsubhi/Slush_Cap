#include <msp430fr5989.h>
#include <stdint.h>
#include "cc1101.h"


#define TX_BUFFER_SIZE 5
void Ports_config();
void digiPot_SPI_config();
void digiPot_SPI_start();
//void AcceldigitalPotWrite();
void RadiodigitalPotWrite();
void PortsINT_config();
void TimerA_config();
void ADC_Pins_config();
uint8_t SPI_transfer(uint8_t _data);
uint8_t spi_send(const uint8_t _data);
uint16_t spi_send2(const uint8_t _data1, const uint8_t _data2);
volatile uint8_t RadioCap = 0;
uint8_t TX_buffer [TX_BUFFER_SIZE] = {0};
uint8_t i;

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;
Ports_config(); //  setting all ports as input with pull down resistors
digiPot_SPI_config();
digiPot_SPI_start();
 // delay(10);
 // AcceldigitalPotWrite();
 delay(10);
 RadiodigitalPotWrite();
 delay(10);
PortsINT_config(); //configure the inturreptable pins
TimerA_config();
 PJDIR |= BIT1; // set Gate PJ.1 as output




for(i=0;i<TX_BUFFER_SIZE;i++)
{
TX_buffer [i] = i;
}




 for(;;)
 {

   __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 w/ interrupts

   if (RadioCap) // if the radio capacitor is fully charged
   {
     //turn on the radio by driving the gate high
      PJOUT |=BIT1;
      delay(100);
      //Radio startup
     Init();
     SetDataRate(4); // Needs to be the same in Tx and Rx
     SetLogicalChannel(1); // Needs to be the same in Tx and Rx
     SetTxPower(0);
     delay(1000);
	  __delay_cycles(100);
	  SendDataNoWait(TX_buffer, TX_BUFFER_SIZE);
    __delay_cycles(100000);
	  //turn off the radio by driving the gate low
	  PJOUT &= ~BIT1;
    RadioCap =0;
    }

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
void digiPot_SPI_config(){

  // setup UCB1 for spi Communication
   UCB1CTLW0 = UCSWRST | UCSSEL_2; // UCSWRST= Put USCI in reset mode, UCSSEL_2 = source USCI clock from SMCLK
   UCB1CTLW0 |= UCCKPH | UCMSB | UCMST | UCSYNC; //UCCKPH = SPI MODE 0(CPOL=0 CPHA=0) , UCMSB= MSB,   UCMST= master mode

   P3SEL0 |= BIT1;  // P3.1-> UCB1SIMO
   P3SEL1 &= ~ BIT1;
   P3SEL0 |= BIT2 ; //P3.2-> UCB1SOMI
   P3SEL1 &= ~ BIT2 ;
   P3SEL0 |= BIT0 ; //P3.0-> USCB1CLK
   P3SEL1 &= ~ BIT0 ;
   // set cs P1.0 as output for cs
   P1DIR |= BIT0;
  // set cs P3.7 as output for cs
   P3DIR |= BIT7;
   // UCB1 set initial speed
   UCB1BR0 |= 0x02;
   UCB1BR1 = 0;
}
//////////////////////////////////////////////////////////
void digiPot_SPI_start(){
 UCB1CTLW0 &= ~UCSWRST; // release UCB1 for operation

}
///////////////////////////////////////////////////////////
void PortsINT_config(){

  P3DIR &= ~BIT3;                    // P3.3 as input
  P3OUT |= BIT3;                      // Configure P3.3 as pulled
  P3REN &= ~BIT3;                      // P3.3  pull-down register enable
  P3IES &= ~BIT3;                  // P3IES-> Port 3 Interrupt Edge Select Register , lo/hi
  P3IFG &= ~BIT3;               // cleared P3IFG-> Port 3 Interrupt Flag Register
  P3IE |= BIT3;                    //P4IE-> Port 3 Interrupt Enable Register,  P3.3 interrupt enabled

}
void ADC_Pins_config(){
  // Configure P1.1 for ADC
  P1SEL1 |= BIT1;
  P1SEL0 |= BIT1;



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
 void AcceldigitalPotWrite() {
   // take the CS pin low to select the chip:
   P1OUT &= ~BIT0;
    delay(10);
   // send writing instruction to WR0
   //SPI_transfer(0b11000000); //  110 writing instruction followed by WR0 address
   //SPI_transfer(0b11111111); // set WR0 to max value
   spi_send2(0b11000000,0b00000000);
   // delay(10);
   // P1OUT |= BIT0;
   // P1OUT &= ~BIT0;
    delay(10);
   //SPI_transfer(0b11000001);  //  110 writing instruction followed by WR1 address
   //SPI_transfer(0b11111111); // set WR1 to min value
   spi_send2(0b11000001,0b00000000);
    delay(10);
   // take the CS pin high to de-select the chip:
   P1OUT |= BIT0;
 }


 void RadiodigitalPotWrite() {
   // take the CS pin low to select the chip:
   P3OUT &= ~BIT7;
    delay(10);
   // send writing instruction to WR0
   // 0b11000000 : 110 writing instruction followed by WR0 address
   // 0b00000000) : set WR0 to max value
   spi_send2(0b11000000,0b0000000);
   delay(10);
   // P1OUT |= BIT0;
   // P1OUT &= ~BIT0;
   //  delay(10);
   //0b11000001: 110 writing instruction followed by WR1 address
   //0b00000000 :set WR1 to max value
   spi_send2(0b11000001,0b00000000);
    delay(10);
   // take the CS pin high to de-select the chip:
   P3OUT |= BIT7;
 }

 uint8_t SPI_transfer(uint8_t _data) {
     return spi_send(_data);
 }



 uint8_t spi_send(const uint8_t _data)
 {
 	/* Wait for previous tx to complete. */
 	while (!(UCB1IFG & UCTXIFG)) // UCB1 flage
 		;

 	/* Setting TXBUF clears the TXIFG flag. */
 	UCB1TXBUF = _data; // transfer buffer for UCB1

 	/* Wait for a rx character? */
 	while (!(UCB1IFG & UCRXIFG)) // UCB1 flage
 		;

 	/* Reading clears RXIFG flag. */
 	return UCB1RXBUF; //reciveing buffer for UCB1
 }

 uint16_t spi_send2(const uint8_t _data1, const uint8_t _data2)
 {
   uint16_t result = 0;
 	/* Wait for previous tx to complete. */
 	while (!(UCB1IFG & UCTXIFG)) // UCB1 flage
 		;

 	/* Setting TXBUF clears the TXIFG flag. */
 	UCB1TXBUF = _data1; // transfer buffer for UCB1

 	/* Wait for a rx character? */
 	while (!(UCB1IFG & UCRXIFG)) // UCB1 flage
 		;

 	/* Reading clears RXIFG flag. */
 	result = UCB1RXBUF; //reciveing buffer for UCB1

  /* Wait for previous tx to complete. */
 	while (!(UCB1IFG & UCTXIFG)) // UCB1 flage
 		;

 	/* Setting TXBUF clears the TXIFG flag. */
 	UCB1TXBUF = _data2; // transfer buffer for UCB1

 	/* Wait for a rx character? */
 	while (!(UCB1IFG & UCRXIFG)) // UCB1 flage
 		;

 	/* Reading clears RXIFG flag. */
 	result = (result << 8) | UCB1RXBUF; //reciveing buffer for UCB1
  return result;
 }


 void __attribute__ ((interrupt(PORT3_VECTOR))) Port_3(void)
 {
   P3IFG &=~BIT3;                        // P3.3 IFG cleared
   RadioCap= 1;
  // __bic_SR_register_on_exit(LPM3_bits);

 }

 void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A(void) // don't use #pragma, it does not work with msbdebug
 {

        __bic_SR_register_on_exit(LPM3_bits);
         TA0CTL |= TACLR; // TACLR-> to clear TAR(Timer A Register), the clock divider logic (the divider setting remains unchanged), and the count direction
         TA0CTL &= ~TAIFG; // TAIFG-> Timer_A interrupt flag, 0b = No interrupt pending

 }
