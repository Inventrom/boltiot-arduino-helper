/* Provided by Inventrom for interfacing bolt to Arduino */

/* 
 * File:   Bolt.h
 * Author: Inventrom
 *
 * Created on 1 September, 2017, 8:00 AM
 */

#ifndef BOLTIOT_H
#define BOLTIOT_H
#include <Stream.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

/* This structure holds all the commands set by the user.
 */

/* Class which handles the communication with bolt over UART
 * The UART can be any hardware serial or software serial port
 */
class BoltIoT {

    class CommandList {
    public:
        String command; //Holds the command to be checked for.
        String(*commandCallback)(String *); // Hold the callback related to the command.
        int numberOfArguments; // Number of arguments that will be sent along with the command.
        int numberOfArgumentsFound; //Number of arguments currently found.
        String* arguments; // String containing the arguments.
        String returnData; //Holds the data returned by the callback, which has to be sent back
        char dataDelimiter; //the character which will separate the command from arguments. Example SetPin;pinNumber=3;pinState=HIGH;
        CommandList *nextCommand; // This forms a linked list, linking to the next command in the list.
    }; //This class is defined here, to ensure that it only works with the Bolt IoT Arduino Helper library 

    Stream *communicationPort; //Holds the communication port at which Bolt is connected.
    String receivedString; //Holds the command currently being passed

    CommandList *commandList;
    bool isCommandReceived(String command);
    bool runCommand(CommandList &commandStruct, char data);
    long argumentReadTimeout;
    CommandList *currentMonitoredCommand;
public:

    BoltIoT();
    /* Used to initialize the bolt class with Hardware Serial port. The function starts the uart at 9600, the required baud rate for bolt
     * uart-Hardware serial port to which Bolt is connected. Can be Serial, Serial0, Serial1
     */
    void begin(HardwareSerial& uart);


    /* Used to initialize the bolt class with Software Serial port. The function starts the software uart at 9600, the required baud rate for bolt
     * Rx_Pin- The arduino pin number to which the Tx pin of Bolt is connected.
     * Tx_Pin- The arduino pin number to which the Rx pin of Bolt is connected.
     */
    void begin(int Rx_Pin, int Tx_Pin);


    /* This function has to be called from the loop function.
     * The CheckPoll function checks for data received on the uart, and confirms with the bolt has sent the read command("RD\r") to the Arduino 
     * If the arduino has received the read command from the bolt, the functions sends back 6 float csv's, which are provided as the variables
     * At the least 1 variable has to be set by the user.
     */
    void CheckPoll(float a, float b = 0, float c = 0, float d = 0, float e = 0, float f = 0);


    /* 
     * @Future_development
     * This function iterates through the whole list of commands vs command functions and calls the command function depending on the command received.
     */

    void handleCommand();

    /* @Future_Development:
     * This string currently only provides the data buffered in the bolt class.
     */
    String getReceivedString();

    /* @Future_Development:
     * This 
     */
    void setCommandString(String command, String(*command_function)(String *), int number_of_arguments = 0, char dataDelimiter = ' ');

    void SetArgumentTimeout(long timeout);
};

extern BoltIoT boltiot; //Variable defined in the header so that user does not have to define the variable
#endif /* BOLT_H */

