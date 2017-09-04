This library reduces the effort required to interface and arduino to a bolt module.
The libraray supports hardware as well as software serial uarts, as such the bolt can be interfaced to the with any pins of the arduino. But it would be more robust to use hardware uart over software uart.
By using this library, any data collected or processed by the arduino can be snet to the bolt cloud, without much programming.


Please follow the following steps to include the bolt library to arduino
1)Go to /*github link*/
2)Click on clone or download button and download the zip
3)Open Arduino Ide.
4)Click on Sketch->Add Library->Add .Zip library.
5)Navigate to the folder where Bolt.zip was downloaded, and select Bolt.zip.
6)Press ok and the library is loaded to your arduino ide.


Example 1 : Interfacing via hardware serial port

#include <Bolt.h>

void setup() {
  bolt.Begin(Serial); //Initialize the bolt interface over serial uart. Serial could be replaced with Serial0 or Serial1 on arduino mega boards.
		   //In this example Tx pin of bolt is connected to rx pin of arduino Serial Port
		   //and rx pin of bolt is connected to tx pin of arduino Serial Port
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the bolt cloud
}

void loop() {
  bolt.CheckPoll(digitalRead(2)); //Send data to the bolt cloud, when the bolt polls the arduino,for data.
/*This function needs to be called reglarly. Calling the CheckPoll function once every seconds is required*/
}



Example 2: Interfacing via software serial port

#include <Bolt.h>

void setup() {
  bolt.Begin(3,4); //Initialize the bolt interface over software serial uart.
		   //In this example Tx pin of bolt is connected to pin 3 of arduino
		   //and rx pin of bolt is connected to pin 4 of arduino
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the bolt cloud
}

void loop() {
  bolt.CheckPoll(digitalRead(2)); //Send data to the bolt cloud, when the bolt polls the arduino,for data.
/*This function needs to be called reglarly. Calling the CheckPoll function once every seconds is required*/
}
