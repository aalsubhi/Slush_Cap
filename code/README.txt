slush.app1:(event detection)
every 2 second the temperature will be sensed and if it's >=30 C,  the notification will be send through radio.

slush.app2:(continuous sensing):
every 2 seconds the wetness of a plant , and the light level will be sensed and send through Radio.

Radio Receiver:
This folder has two versions of receiver software. one for each application, since each application sending different size of packet.

testCode:
has some codes to test different parts on different MCU such as radio,and ADC reading. 
