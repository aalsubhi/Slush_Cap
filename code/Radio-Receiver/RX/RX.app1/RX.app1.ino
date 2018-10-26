#include "cc1101.h"
#include "macros.h"
#include "pins.h"
#include "registers.h"
#define LED RED_LED
unsigned long lastrx; 
unsigned long elapsed; 
void setup()
{
  pinMode(LED, OUTPUT);   
  Serial.begin(9600);
  delay(1000);
  Radio.Init();
  Radio.SetDataRate(4); // Needs to be the same in Tx and Rx
  Radio.SetLogicalChannel(1); // Needs to be the same as receiver
	lastrx=millis();
  Serial.println("done init");
  Radio.RxOn();
  
}

byte RX_buffer[4]={0};
byte sizerx,i,flag;

void loop()
{
  if(Radio.CheckReceiveFlag())
  {
    lastrx = millis();
    sizerx=Radio.ReceiveData(RX_buffer);
    // Should be one byte
    //Serial.write(RX_buffer[0])
   int32_t   tempreture  = ((int32_t)(RX_buffer[3]) << 24)
              + ((int32_t)(RX_buffer[2]) << 16)
              + ((int32_t)(RX_buffer[1]) << 8)
              + ((int32_t)(RX_buffer[0]));
    Serial.println( "got something");
   // Serial.println( RX_buffer[0], DEC);
   // Serial.println( RX_buffer[1], DEC);
    //Serial.println( RX_buffer[2], DEC);
    //Serial.println( RX_buffer[3], DEC);
   
    Serial.println(tempreture,DEC );
     
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
   delay(1000);               // wait for a second  
   digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
   
    Radio.RxOn();
  }
}


