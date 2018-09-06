# Description
This library reduces the effort required to interface an Arduino to a Bolt module.
The library supports hardware as well as software serial UARTs, as such the Bolt can be interfaced to with any pins of the Arduino. But it would be more robust to use hardware UART over software UART.
By using this library, any data collected or processed by the Arduino can be sent to the Bolt Cloud.

# Installation steps
Please follow the steps to include the BoltIoT-Arduino-Helper library to Arduino
1. Click [here](https://github.com/Vinayakj009/boltiot-arduino-helper/archive/master.zip) to download the zip
2. Open Arduino IDE.
3. Click on ```Sketch->Include Library->Add .Zip library```.
4. Navigate to the folder where boltiot-arduino-helper-master.zip was downloaded, and select boltiot-arduino-helper-master.zip.
5. Press the 'Choose' button and the library is loaded to your Arduino IDE.

# (Optional) Defining global API key and device id (Details of why this is needed will be added later)
While it is a good idea to define the API key and device id as a part of the code, it is sometimes troublesome to write the corresponding code everytime you have a new project.
You can define a global API key or device id, by using the following steps.
1. Go to the ```libraries/boltiot-arduino-helper-master/src``` folder inside your sketchbook folder. The sketchbook folder is normally ```My Documents/Arduino``` folder for windows and ```~/Documents/Arduino/``` folder in Mac and Linux. 
2. Open the file ```BoltDeviceCredentials.h``` file in a text editor.
3. Uncomment API_KEY or DEVICE_ID or both macros, whichever you want to make common across all examples.
4. Replace the dummy values with actual value for the corresponding macro.
5. Save and close the file.

You are done.
Now all you have to do is include the ```BoltDeviceCredentials.h``` header into any code where the macros are required.
You can do this by including the following line at the top of your code.
```cpp
#include <BoltDeviceCredentials.h>
```

Please Note: You can always overwrite the global value by defining the API_KEY and DEVICE_ID macro within your code. This can be done by adding the code snippet below in your code and accordingly updating the values.
```cpp
#define API_KEY     "xxxxxx-xxxx-xxxx-xxxx-xxxxxx"
#define DEVICE_ID   "BOLTxxxxxxxx"
```

Please Note: The definitions in the examples will only take effect, if the corresponding definitions are not already done in the ```BoltDeviceCredentials.h``` header file.
# Available functions
## begin
This function helps to initialize the boltiot variable with a UART communications interface. The communication interface can be a hardware serial interface or a software serial interface.
You can read more about hardware serial interface by clicking [here](https://www.arduino.cc/reference/en/language/functions/communication/serial/), and software serial interface by clicking [here](https://www.arduino.cc/en/Reference/SoftwareSerial).

To use a hardware serial interface with the Bolt unit, use the following code snippet in the setup function of your Arduino code. In the code snippet, we are using 'Serial' as the hardware serial interface. This interface is available on most Arduino boards. To use this hardware serial interface, connect the TX pin of the Bolt to the RX pin of your Arduino, and the RX pin of the Bolt to the TX pin of your Arduino. Some boards such as the Arduino 
also have Serial0 and Serial1 as hardware serial interfaces. Pin connections with the Bolt differ based on the hardware serial interface used.
```cpp
boltiot.begin(Serial);
```

To use a software serial interface with the Bolt unit, use the following code snippet in the setup function of your Arduino code. In the following example, you will need to connect 
pin of the Bolt to pin 3 of the Arduino, and RX pin of the Bolt to pin 4 of the Arduino. You can choose which pins to connect to the TX and RX pins of the Bolt, and accordingly replace the pin number in the code below.
```cpp
boltiot.begin(3,4);
```
The begin function initializes the serial interface (hardware or software) to communicate at a baud rate of 9600, which is most commonly used with the Bolt.

## processPushDataCommand
This function monitors the serial interface for the "RD\r" command, sent by the Bolt unit, and responds to the Bolt via the serial interface, with the data supplied to the function.<br>
Use this function if the only function that the Arduino will do, is log data to the Bolt Cloud.<br>

This function requires a minimum of 1 argument and can take up to 6 arguments. These arguments can be of the type int or float. The following examples are valid ways to use this function.
The number of arguments you pass, corresponds to the number of different sensor data you want to log to the Cloud.<br>
Please Note: The number of arguments you pass to this function should be the same as the number of CSV values you configure for the corresponding UART product on the Bolt Cloud.
```cpp
boltiot.processPushDataCommand(2);
```

```cpp
int sensorData=20;
boltiot.processPushDataCommand(sensorData);
```

```cpp
int sensorData1=20;
float sensorData2=20.6;
boltiot.processPushDataCommand(sensorData1,sensorData2);
```

```cpp
int integerSensorData[3]={20,24,26};
float floatSensorData[3]={20.6,-10.5,30.4};
boltiot.processPushDataCommand(integerSensorData[0],floatSensorData[2],floatSensorData[1],integerSensorData[2],integerSensorData[1],floatSensorData[0]);
```
This function has to be called at the least once per second for it to work properly. Use this function in the loop function of your Arduino code.

The function returns a 'true' when it receives the "RD\r" command from the Bolt, and false when it does not receive the "RD\r" command from the Bolt.

To learn more about this function read the example given in File>Examples>BoltIoT-Arduino-Helper>PushData in your Arduino IDE.

## setCommandString
***If you use this function, remember to also use the handleCommand function***<br>
If you want to use the Bolt Cloud to send commands to the Arduino, or you want to use the Arduino along with the Bolt and the Bolt Cloud to send custom data to your own Cloud, you need to use this function.

This function allows you to tell the library, to execute a specific command function when the Arduino receives a specific command from the Bolt. The setCommandString function stores the command function along with the associated command string within the library.
For example, if you want the Arduino to send analog values of all analog input pins when the Bolt sends the command "getAnalogData" via the serial interface, you will use the following code, in your setup function.
```cpp
boltiot.setCommandString("GetAnalogData",getAnalogData);
```

and define the following command function separately outside the setup function.
```cpp
String getAnalogData(String *data){
	String retval="";
	retval=retval+analogRead(A0)+",";
	retval=retval+analogRead(A1)+",";
	retval=retval+analogRead(A2)+",";
	retval=retval+analogRead(A3)+",";
	retval=retval+analogRead(A4)+",";
	retval=retval+analogRead(A5);
	return retval;
}
```

To get the analog data, you will have to run the SerialWrite API call from the Bolt Cloud with the data "getAnalogData" and then run the SerialRead API call from the Bolt Cloud to fetch the data sent by the Arduino.
For example you will have to run the following link via the Web Browser, to send the command to the Arduino via the serial interface of the Bolt.
```
https://cloud.boltiot.com/remote/{API_KEY}/serialWrite?data=getAnalogData&deviceName={DEVICE_ID}
```
and then run the following link via the Web Browser, to read the data sent by the Arduino via the serial interface to the Bolt unit.
```
https://cloud.boltiot.com/remote/{API_KEY}/serialRead?deviceName={DEVICE_ID}
```
To learn more about the SerialRead API call click [here](https://docs.boltiot.com/docs/read-incoming-serial-data) and SerialWrite API call click [here](https://docs.boltiot.com/docs/send-serial-data-output).

To learn more about this function read the example given in File>Examples>BoltIoT-Arduino-Helper>CommandHandler in your Arduino IDE.

Please Note: To have the functions called when the corresponding command is received from the Bolt, you need to call the handleCommand function in the loop function at the least once every second.

### Things to remember while defining your own custom command function
While the library allows you to use and function as a command function with the setCommandString function allow you to successfully compile the code, the code **will not function properly** if the following rules are not followed.
 * The command function **should only return a string**.
 * The command function should take a string pointer as input.
 
Here is a code snippet you can use in your code. Remember to change the function name "ExampleCustomCommandFunction" to the function name you want to use, and write the custom command code in the indicated area.

 ```cpp
 String ExampleCustomCommandFunction(String *data){
 	String returnString="";
	//Write your custom command code here.
	return returnString;
 }
 ```

## handleCommand
***Only use this function if you have already used the setCommandString function***<br>
***This funciton has to be called once every second for it to function properly***<br>
The handleCommand function is equivalent to the processPushDataCommand function.<br>
This function checks for new data from the serial interface compares it to the command strings within the library. If the received command matches a command string, the associated command function is executed.<br>
The function returns the number of commands executed, every time it is called.<br>

You can call this function in the loop function of the Arduino code, in the following manner.
```cpp
boltiot.handleCommand();
```

# Examples
Remember to go through the Examples packaged along with this library. They contain better explanations of how to use the Command Response model for this library.
## Interfacing via hardware serial port
```cpp
#include <BoltIoT-Arduino-Helper.h>

void setup() {
  boltiot.begin(Serial); //Initialize the Bolt interface over serial UART. Serial could be replaced with Serial0 or Serial1 on Arduino Mega boards.
		   //In this example, the TX pin of Bolt is connected to the RX pin of Arduino Serial Port
		   //and the TX pin of Bolt is connected to the TX pin of Arduino Serial Port
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
		   //In this example, the TX pin of Bolt is connected to pin 3 of Arduino
		   //and the RX pin of Bolt is connected to pin 4 of Arduino
  pinMode(2,INPUT); //Set pin 2 as the input. We will use send this pin's state as the data to the Bolt cloud
}

void loop() {
  boltiot.processPushDataCommand(digitalRead(2)); //Send data to the Bolt cloud, when the Bolt polls the Arduino for data.
/*This function needs to be called regularly. Calling the processPushDataCommand function once every second is required*/
}
```

