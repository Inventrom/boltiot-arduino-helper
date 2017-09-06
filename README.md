# Description
This library reduces the effort required to interface and Arduino to a Bolt module.
The library supports hardware as well as software serial UARTs, as such the Bolt can be interfaced to with any pins of the Arduino. But it would be more robust to use hardware UART over software UART.
By using this library, any data collected or processed by the Arduino can be sent to the Bolt Cloud.

# Installation steps
Please follow the steps to include the bolt library to arduino
1. Click on clone or download button and download the zip
2. Open Arduino IDE.
3. Click on Sketch->Add Library->Add .Zip library.
4. Navigate to the folder where boltiot-arduino-helper.zip was downloaded, and select boltiot-arduino-helper.zip.
5. Press ok and the library is loaded to your arduino ide.

# Examples
## Interfacing via hardware serial port
```
#include <BoltIot-Arduino-Helper.h>

void setup() {
  boltiot.Begin(Serial); //Initialize the bolt interface over serial uart. Serial could be replaced with Serial0 or Serial1 on arduino mega boards.
		   //In this example Tx pin of bolt is connected to rx pin of arduino Serial Port
		   //and rx pin of bolt is connected to tx pin of arduino Serial Port
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the bolt cloud
}

void loop() {
  boltiot.CheckPoll(digitalRead(2)); //Send data to the bolt cloud, when the bolt polls the arduino,for data.
/*This function needs to be called reglarly. Calling the CheckPoll function once every seconds is required*/
}
```


## Interfacing via software serial port
```
#include <BoltIot-Arduino-Helper.h>

void setup() {
  boltiot.Begin(3,4); //Initialize the bolt interface over software serial uart.
		   //In this example Tx pin of bolt is connected to pin 3 of arduino
		   //and rx pin of bolt is connected to pin 4 of arduino
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the bolt cloud
}

void loop() {
  boltiot.CheckPoll(digitalRead(2)); //Send data to the bolt cloud, when the bolt polls the arduino,for data.
/*This function needs to be called reglarly. Calling the CheckPoll function once every seconds is required*/
}
```
