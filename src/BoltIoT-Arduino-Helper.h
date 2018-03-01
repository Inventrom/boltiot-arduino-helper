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
    /*
     * These are the states of the library monitoring incoming commands.
     */
    static const int DiscardCurrentData=-2;
    static const int WaitForFirstDelimiter=-1;

    /*
     * This class acts like a linked list and holds all data related to all configured commands.
     */
    class CommandList {
    public:
        String command; //Holds the command to be checked for.
        String(*commandCallback)(String *); // Hold the callback related to the command.
        int numberOfArgumentsExpected; // Number of arguments that will be sent along with the command.
        int numberOfArgumentsReceived; //Number of arguments currently found.
        String* arguments; // String containing the arguments.
        String returnData; //Holds the data returned by the callback, which has to be sent back
        char dataDelimiter; //the character which will separate the command from arguments. Example SetPin;pinNumber=3;pinState=HIGH;
        CommandList *nextCommand; // This forms a linked list, linking to the next command in the list.
    }; //This class is defined here, to ensure that it only works with the Bolt IoT Arduino Helper library 

    Stream *communicationPort; //Holds the communication port at which Bolt is connected.
    String receivedString; //Holds the command currently being passed
    CommandList *commandList; //Linked list which holds all configured commands, along with their associated settings.
    CommandList *currentMonitoredCommand; //The command which has just been received, and the library is waiting for its arguments.
    /*
     * This variable will be used in the future to hold the time for which the library will hold incoming data, which is not part of any command.
     * long readTimeout;
     */ 
    
    /*
     * This functions checks if the given command has been received or not.
     * Arguments:
     * command - This is the command which needs to be checked for.
     */
    bool isCommandReceived(String command);
    
    /*
     * This function checks for arguments and runs the supplied command if all 
     * the command arguments are received. IF the command does not require any 
     * arguments, the command is run immediately.
     * 
     * Arguments: 
     * commandSettings - This is an instance of CommandList, and holds all
     *                   settings associated with the command.
     * data - This is the character that has just been pulled from the Bolt.
     * 
     * Returns : true if the command was executed.
     */
    bool runCommand(CommandList &commandSettings, char data);
    
    /*
     * This function sends individual values of the CSV string. This function
     * can only be used after the first variable in the CSV string is sent.
     * 
     * Arguments:
     * isFirstVar - This tells the function whether the variable is the first,
     *              among the CSV string. If true, the function will not print 
     *              a ',' before the value.
     * var - The value to be send as a part of the CSV string.
     * sendNewLine - Whether to send a new line after sending the variable.
     */
    void sendVar(bool isFirstVar,float var,String sendNewLine);
public:

    BoltIoT();

    /* This function is deprecated, and is available for backward compatability.
     * Please use the begin(int Rx_Pin, int Tx_Pin) function instead
     */
    void Begin(int Rx_Pin, int Tx_Pin);
    
    
    /* This function is deprecated, and is available for backward compatability.
     * Please use the begin(HardwareSerial& uart) function instead
     */
    void Begin(HardwareSerial& uart);


    /* Used to initialize the bolt class with Software Serial port.
     * The function starts the software uart at 9600, the required baud rate for
     * bolt
     * Arguments:
     * Rx_Pin- The Ardino pin number to which the Tx pin of Bolt is connected.
     * Tx_Pin- The Ardino pin number to which the Rx pin of Bolt is connected.
     */
    void begin(int Rx_Pin, int Tx_Pin);
    
    
    /* Used to initialize the bolt class with Hardware Serial port.
     * The function starts the uart at 9600, the required baud rate for bolt.
     * Arguments:
     * uart - Hardware serial port to which Bolt is connected. 
     *        Can be Serial, Serial0, Serial1
     */
    void begin(HardwareSerial& uart);


    /* This function is deprecated.
     * If possible use the processPushDataCommand function instead.
     */
    void CheckPoll(float var1, float var2 = 0, float var3 = 0, float var4 = 0, float var5 = 0, float var6 = 0);
    
    /*
     * This function has to be called from the loop function.
     * The processPushDataCommand function checks for data received on the uart
     * and confirms with the bolt has sent the read command("RD\\r") 
     * to the Arduino 
     * 
     * If the Ardino has received the read command from the bolt, 
     * the functions sends back 1 float value, which is provided
     * as the variable
     * 
     * 
     * Arguments:
     * var1 - This is the 1st variable in the CSV string sent to the Bolt.
     * sendNewLine - Optional argument. This tells the function whether to send
     *               a new line at the end of the CSV string. 
     *               Default value is "true"
     * 
     * Returns : True, if command was received, and data was sent.
     */
    bool processPushDataCommand(float var1, String sendNewLine = "true");
    
    /*
     * This function has to be called from the loop function.
     * The processPushDataCommand function checks for data received on the uart
     * and confirms with the bolt has sent the read command("RD\\r") 
     * to the Arduino 
     * 
     * If the Ardino has received the read command from the bolt, 
     * the functions sends back 2 float csv's, which are provided
     * as the variables
     * 
     * 
     * Arguments:
     * var1 - This is the 1st variable in the CSV string sent to the Bolt.
     * var2 - This is the 2nd variable in the CSV string sent to the Bolt.
     * sendNewLine - Optional argument. This tells the function whether to 
     *               send a new line at the end of the CSV string. 
     *               Default value is "true"
     * 
     * Returns : True, if command was received, and data was sent.
     */
    bool processPushDataCommand(float var1, float var2, String sendNewLine = "true");
    
    /*
     * This function has to be called from the loop function.
     * The processPushDataCommand function checks for data received on the uart
     * and confirms with the bolt has sent the read command("RD\\r") 
     * to the Arduino 
     * 
     * If the Ardino has received the read command from the bolt, 
     * the functions sends back 3 float csv's, which are provided
     * as the variables
     * 
     * 
     * Arguments:
     * var1 - This is the 1st variable in the CSV string sent to the Bolt.
     * var2 - This is the 2nd variable in the CSV string sent to the Bolt.
     * var3 - This is the 3rd variable in the CSV string sent to the Bolt.
     * sendNewLine - Optional argument. This tells the function whether to 
     *               send a new line at the end of the CSV string. 
     *               Default value is "true"
     * 
     * Returns : True, if command was received, and data was sent.
     */
    bool processPushDataCommand(float var1, float var2, float var3, String sendNewLine = "true");
    
    /*
     * This function has to be called from the loop function.
     * The processPushDataCommand function checks for data received on the uart
     * and confirms with the bolt has sent the read command("RD\\r") 
     * to the Arduino 
     * 
     * If the Ardino has received the read command from the bolt, 
     * the functions sends back 4 float csv's, which are provided
     * as the variables
     * 
     * 
     * Arguments:
     * var1 - This is the 1st variable in the CSV string sent to the Bolt.
     * var2 - This is the 2nd variable in the CSV string sent to the Bolt.
     * var3 - This is the 3rd variable in the CSV string sent to the Bolt.
     * var4 - This is the 4th variable in the CSV string sent to the Bolt.
     * sendNewLine - Optional argument. This tells the function whether to 
     *               send a new line at the end of the CSV string. 
     *               Default value is "true"
     * 
     * Returns : True, if command was received, and data was sent.
     */
    bool processPushDataCommand(float var1, float var2, float var3, float var4, String sendNewLine = "true");
    
    /*
     * This function has to be called from the loop function.
     * The processPushDataCommand function checks for data received on the uart
     * and confirms with the bolt has sent the read command("RD\\r") 
     * to the Arduino 
     * 
     * If the Ardino has received the read command from the bolt, 
     * the functions sends back 5 float csv's, which are provided
     * as the variables
     * 
     * 
     * Arguments:
     * var1 - This is the 1st variable in the CSV string sent to the Bolt.
     * var2 - This is the 2nd variable in the CSV string sent to the Bolt.
     * var3 - This is the 3rd variable in the CSV string sent to the Bolt.
     * var4 - This is the 4th variable in the CSV string sent to the Bolt.
     * var5 - This is the 5th variable in the CSV string sent to the Bolt.
     * sendNewLine - Optional argument. This tells the function whether to 
     *               send a new line at the end of the CSV string. 
     *               Default value is "true"
     * 
     * Returns : True, if command was received, and data was sent.
     */
    bool processPushDataCommand(float var1, float var2, float var3, float var4, float var5, String sendNewLine = "true");
    
    /*
     * This function has to be called from the loop function.
     * The processPushDataCommand function checks for data received on the uart
     * and confirms with the bolt has sent the read command("RD\\r") 
     * to the Arduino 
     * 
     * If the Ardino has received the read command from the bolt, 
     * the functions sends back 6 float csv's, which are provided
     * as the variables. The CSV string is padded with a newline character.
     * 
     * Arguments:
     * var1 - This is the 1st variable in the CSV string sent to the Bolt.
     * var2 - This is the 2nd variable in the CSV string sent to the Bolt.
     * var3 - This is the 3rd variable in the CSV string sent to the Bolt.
     * var4 - This is the 4th variable in the CSV string sent to the Bolt.
     * var5 - This is the 5th variable in the CSV string sent to the Bolt.
     * var6 - This is the 6th variable in the CSV string sent to the Bolt.
     * 
     * Returns : True, if command was received, and data was sent.
     */
    bool processPushDataCommand(float var1, float var2, float var3, float var4, float var5, float var6);


    /* 
     * This function iterates through the whole list of commands
     * and calls the command function depending on the commands received.
     * 
     * Returns : Number of commands executed.
     * 
     */

    int handleCommand();

    /* 
     * This function returns the data that has been received over uart,
     * but has not been processed to be a part of any configured command.
     * 
     * Returns : The data currently present in the library's buffer.
     */
    String getReceivedString();

    /* 
     * This function tells the library which commands to look out for,
     * and what function to call as a response.
     * 
     * Arguments:
     * command - This is the command that the library will search for in the 
     *           incoming data.
     * callbackFunction - This is the function which will be called when the
     *                    associated command is received along with all it's 
     *                    arguments.
     * numberOfArguments - Optional argument. The number of arguments 
     *                     that the library should look for along with
     *                     the associated command.
     *                     Default value is 0.
     * dataDelimiter - Optional argument. The character which separates the
     *                 associated command from it's arguments. The library 
     *                 will expect this character at the end of all arguments
     *                 as well.
     *                 Default value is ' ' (Thats a space).
     * 
     */
    void setCommandString(String command, String(*callbackFunction)(String *), int numberOfArguments= 0, char dataDelimiter = ' ');

    /*
     * This function is kept for future development. The timeout value tells the library how long a received string has to be maintained.
     * 
     * void setTimeout(long timeout);
     * 
     * This function is kept for future development. This function will allow the application to disable any command path, that were previously configured.
     * 
     * void removeCommandString(String command);
     */ 
};

extern BoltIoT boltiot; //Variable defined in the header so that user does not have to define the variable
#endif /* BOLTIOT_H */

