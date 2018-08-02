# Description
This library reduces the effort required to interface and Arduino to a Bolt module.
The library supports hardware as well as software serial UARTs, as such the Bolt can be interfaced to with any pins of the Arduino. But it would be more robust to use hardware UART over software UART.
By using this library, any data collected or processed by the Arduino can be sent to the Bolt Cloud.

# Installation steps
Please follow the steps to include the bolt library to arduino
1. Click on clone or download button and download the zip
2. Open Arduino IDE.
3. Click on Sketch->Add Library->Add .Zip library.
4. Navigate to the folder where boltiot-arduino-helper-master.zip was downloaded, and select boltiot-arduino-helper-master.zip.
5. Press ok and the library is loaded to your arduino ide.

# (Optional) Defining global API key and device id (Details of why this is needed will be added later)
While it is a good idea to define the API key and device id as a part of the code, it is sometimes troublesome to write the corresponding code everytime you have a new project.
You can define a global API key or device id, by using the following steps.
1. Go to the 'libraries/boltiot-arduino-helper-master/src' folder inside your sketchbook folder. The sketchbook folder is normally 'My Documents/Arduino' folder for windows and "~/Documents/Arudino/" folder in Mac and linux. 
2. Open the file 'BoltDeviceCredentials.h' file in a text editor.
3. Uncomment API_KEY or DEVICE_ID or both macros, whichever you want to make common across all examples.
4. Replace the dummy values with actual value for the corresponding macro.
5. Save and close the file.

You are done.
Now all you have to is include the 'BoltDeviceCredentials.h' header into any code where the macro are required.

Please Note: You can always overwrite the global value by defining the API_KEY and DEVICE_ID macro within your code. The definitions in the examples will only take effect, if the corresponding definitions are not already done in the 'BoltDeviceCredentials.h' header file.

# Examples
Remeber to go through the Examples packaged along with this library. They contain a better explaination of how to use the Command Response model for this library.
## Interfacing via hardware serial port
```
#include <BoltIoT-Arduino-Helper.h>

void setup() {
  boltiot.Begin(Serial); //Initialize the bolt interface over serial uart. Serial could be replaced with Serial0 or Serial1 on arduino mega boards.
		   //In this example Tx pin of bolt is connected to rx pin of arduino Serial Port
		   //and rx pin of bolt is connected to tx pin of arduino Serial Port
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the bolt cloud
}

void loop() {
  boltiot.processPushDataCommand(digitalRead(2)); //Send data to the bolt cloud, when the bolt polls the arduino,for data.
/*This function needs to be called reglarly. Calling the processPushDataCommand function once every second is required*/
}
```


## Interfacing via software serial port
```
#include <BoltIoT-Arduino-Helper.h>

void setup() {
  boltiot.Begin(3,4); //Initialize the bolt interface over software serial uart.
		   //In this example Tx pin of bolt is connected to pin 3 of arduino
		   //and rx pin of bolt is connected to pin 4 of arduino
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the bolt cloud
}

void loop() {
  boltiot.processPushDataCommand(digitalRead(2)); //Send data to the bolt cloud, when the bolt polls the arduino,for data.
/*This function needs to be called reglarly. Calling the processPushDataCommand function once every second is required*/
}
```

