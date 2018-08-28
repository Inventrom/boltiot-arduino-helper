# Description
This library reduces the effort required to interface and Arduino to a Bolt module.
The library supports hardware as well as software serial UARTs, as such the Bolt can be interfaced to with any pins of the Arduino. But it would be more robust to use hardware UART over software UART.
By using this library, any data collected or processed by the Arduino can be sent to the Bolt Cloud.

# Installation steps
Please follow the steps to include the BoltIoT-Arduino-Helper library to arduino
1. Click [here](https://github.com/Vinayakj009/boltiot-arduino-helper/archive/master.zip) to download the zip
2. Open Arduino IDE.
3. Click on ```Sketch->Include Library->Add .Zip library```.
4. Navigate to the folder where boltiot-arduino-helper-master.zip was downloaded, and select boltiot-arduino-helper-master.zip.
5. Press the 'Choose' button and the library is loaded to your Arduino IDE.

# (Optional) Defining global API key and device id (Details of why this is needed will be added later)
While it is a good idea to define the API key and device id as a part of the code, it is sometimes troublesome to write the corresponding code everytime you have a new project.
You can define a global API key or device id, by using the following steps.
1. Go to the ```libraries/boltiot-arduino-helper-master/src``` folder inside your sketchbook folder. The sketchbook folder is normally ```My Documents/Arduino``` folder for windows and ```~/Documents/Arduino/``` folder in Mac and linux. 
2. Open the file ```BoltDeviceCredentials.h``` file in a text editor.
3. Uncomment API_KEY or DEVICE_ID or both macros, whichever you want to make common across all examples.
4. Replace the dummy values with actual value for the corresponding macro.
5. Save and close the file.

You are done.
Now all you have to do is include the ```BoltDeviceCredentials.h``` header into any code where the macro are required.
You can do this by including the following line at the top of your code.
```cpp
#include <BoltDeviceCredentials.h>
```

Please Note: You can always overwrite the global value by defining the API_KEY and DEVICE_ID macro within your code. This can be done by adding the code below in your code, and accordingly updating the values.
```cpp
#define API_KEY     "xxxxxx-xxxx-xxxx-xxxx-xxxxxx"
#define DEVICE_ID   "BOLTxxxxxxxx"
```

Please Note: The definitions in the examples will only take effect, if the corresponding definitions are not already done in the 'BoltDeviceCredentials.h' header file.
# Available functions
## begin
This function helps initialize the boltiot variable with a UART communications interface. The communication interface can be hardware serial interface, or a software serial interface.
You can read more about hardware serial interface by clicking [here](https://www.arduino.cc/reference/en/language/functions/communication/serial/), and software serial interface by clicking [here](https://www.arduino.cc/en/Reference/SoftwareSerial).

To use a hardware serial interface with the Bolt unit, use the following code snippet in the setup function of your Arduino code. In the code snippet, we are using 'Serial' as the hardware serial interface. This interface is available on most Arduino board. To use this hardware serial interface, connect the TX pin of the Bolt to the TX pin of your Arduino, and the RX pin of the Bolt to the RX pin of your Arduino. Some boards such as the Arduino mega also have Serial0 and Serial1 as hardware serial interfaces. Pin connectins with the Bolt differ based on the hardware serial interface used.
```cpp
boltiot.begin(Serial);
```

To use a software serial interface with the Bolt unit, use the following code snippet in the setup function of your Arduino code. In the following example, you will need to connect TX pin of the Bolt to pin 3 of the Arduino, and RX pin of the Bolt to pin 4 of the Arduino. You can choose which pins to connect to the TX and RX pins of the Bolt, and accordingly replace the pin number in the code below.
```cpp
boltiot.begin(3,4);
```

The begin function initializes the serial interface (hardware or software) to communicate at a baud rate of 9600, which is most commonly used with the Bolt.
# Examples
Remember to go through the Examples packaged along with this library. They contain better explanations of how to use the Command Response model for this library.
## Interfacing via hardware serial port
```cpp
#include <BoltIoT-Arduino-Helper.h>

void setup() {
  boltiot.begin(Serial); //Initialize the Bolt interface over serial UART. Serial could be replaced with Serial0 or Serial1 on Arduino mega boards.
		   //In this example TX pin of Bolt is connected to RX pin of Arduino Serial Port
		   //and Tx pin of Bolt is connected to TX pin of Arduino Serial Port
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the Bolt cloud
}

void loop() {
  boltiot.processPushDataCommand(digitalRead(2)); //Send data to the Bolt cloud, when the Bolt polls the Arduino for data.
/*This function needs to be called regularly. Calling the processPushDataCommand function once every second is required*/
}
```


## Interfacing via software serial port
```cpp
#include <BoltIoT-Arduino-Helper.h>

void setup() {
  boltiot.begin(3,4); //Initialize the Bolt interface over software serial UART.
		   //In this example TX pin of Bolt is connected to pin 3 of Arduino
		   //and RX pin of Bolt is connected to pin 4 of Arduino
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the Bolt cloud
}

void loop() {
  boltiot.processPushDataCommand(digitalRead(2)); //Send data to the Bolt cloud, when the Bolt polls the Arduino for data.
/*This function needs to be called regularly. Calling the processPushDataCommand function once every second is required*/
}
```

