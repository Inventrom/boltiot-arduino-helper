/* Provided by Inventrom for interfacing bolt to Arduino */
#include <Arduino.h>
#include "BoltIoT-Arduino-Helper.h"

BoltIoT boltiot;

BoltIoT::BoltIoT() {
    this->commandList = NULL;// Initialize the list of commands to be null
}

void BoltIoT::Begin(HardwareSerial& uart){
    this->begin(uart);// Use non deprecated function with the same internal working.
}

void BoltIoT::Begin(int Rx_Pin, int Tx_Pin){
    this->begin(Rx_Pin,Tx_Pin);// Use non deprecated function with the same internal working
}

void BoltIoT::begin(HardwareSerial& uart) {
    this->communicationPort = &uart; //Set hardware serial as the default communication port
    ((HardwareSerial*)this->communicationPort)->begin(9600);  //Set the baud rate of the communication port to 9600
}

void BoltIoT::begin(int Rx_Pin, int Tx_Pin) {
    this->communicationPort = new SoftwareSerial(Rx_Pin, Tx_Pin); //Initialize a new software serial port as the communication port.
    ((SoftwareSerial*)this->communicationPort)->begin(9600);      //Set the baud rate of the communication port to 9600.
    ((SoftwareSerial*)this->communicationPort)->listen();         //Enable the communication port to listen on incoming data.
}

void BoltIoT::sendVar(bool isFirstVar,float var, String isLastVar){
    if(!isFirstVar){//Check if the variable is the first in the CSV string.
        this->communicationPort->print(","); //If not send a ','
    }
    this->communicationPort->print(String(var));    // Send the variable
    if(isLastVar.equals("true")){//Check if this is the last variable to be sent.
        this->communicationPort->print("\n"); //If so then send a newline along with 
    }
}

bool BoltIoT::processPushDataCommand(float var1, String isLastVar) {
    while (this->communicationPort->available() > 0) { //Check if new data is available 
        char data = this->communicationPort->read(); //Read a single character from the communication port
        this->receivedString += data;//Append the character received with the received string.
        if (this->receivedString.endsWith("RD\r")) {//Check if RD\r has been received just now.
            this->sendVar(true, var1, isLastVar); //If so then send the first variable out.
            this->receivedString=""; //Set the received string to "", so that it does not interfere with new incoming commands.
            return true;//Return true to indicate that data has been sent.
        }
    }
    return false;//Return false to indicate that data was not pushed.
}

bool BoltIoT::processPushDataCommand(float var1, float var2, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, "false"); //Try to push the first variables
    if (sendPushData) { //Check if push data command was received, and first variable was pushed. 
        this->sendVar(false, var2, isLastVar); //If so push the second variable.
        return true;//Return true to indicate that data was pushed.
    }
    return false;//Return false to indicate that data was not pushed.
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, var2, "false");//Try to push the first 2 variables
    if (sendPushData) {//Check if push data command was received, and first variable was pushed. 
        this->sendVar(false, var3, isLastVar);//If so push the third variable.
        return true;//Return true to indicate that data was pushed.
    }
    return false;//Return false to indicate that data was not pushed.
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, var2, var3, "false");//Try to push the first 3 variables
    if (sendPushData) {//Check if push data command was received, and first variable was pushed. 
        this->sendVar(false, var4, isLastVar);//If so push the fourth variable.
        return true;//Return true to indicate that data was pushed.
    }
    return false;//Return false to indicate that data was not pushed.
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, float var5, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, var2, var3, var4, "false");//Try to push the first 4 variables
    if (sendPushData) {//Check if push data command was received, and first variable was pushed. 
        this->sendVar(false, var5, isLastVar);//If so push the fifth variable.
        return true;//Return true to indicate that data was pushed.
    }
    return false;//Return false to indicate that data was not pushed.
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, float var5, float var6) {
    bool sendPushData = this->processPushDataCommand(var1, var2, var3, var4, var5, "false");//Try to push the first 5 variables
    if (sendPushData) {//Check if push data command was received, and first variable was pushed. 
        this->sendVar(false, var6, "true");//If so push the sixth variable.
        return true;//Return true to indicate that data was pushed.
    }
    return false;//Return false to indicate that data was not pushed.
}

void BoltIoT::CheckPoll(float var1, float var2, float var3, float var4, float var5, float var6) {
    this->processPushDataCommand(var1, var2, var3, var4, var5, var6); //Use new function to process the deprecated function.
}

String BoltIoT::getReceivedString() {
    return this->receivedString;
}

void BoltIoT::setCommandString(String command, String(*commandCallback)(String*), int numberOfArguments, char dataDelimiter) {
    CommandList *parsingPointer; //Declare a temporary pointer to parse through the list.
    parsingPointer = this->commandList; //Initialize the temporary pointer to the top of the command list.
    if (this->commandList == NULL) { //Check if command list is empty
        this->commandList = new CommandList(); //if so create a new instance of the class to hold the newly input command.
        parsingPointer = this->commandList; //Reassign the temporary pointer, as the start of the command list is no longer null
        parsingPointer->nextCommand = NULL; //Set the end of the command list as null.
    } else {//If the command list is not empty, the temporary pointer should parse to the end of the list.
        while ((parsingPointer->nextCommand != NULL)&&(!parsingPointer->command.equals(command))) { //Check if the current command is the last, or the incoming command is the same as current command.
            parsingPointer = parsingPointer->nextCommand; //Move to the next command in the list.
        }
        if (!parsingPointer->command.equals(command)) { //Confirm if the incoming command is not present in the list.
            parsingPointer->nextCommand = new CommandList(); //If the command is not present create a new instance of Command list to hold the setting of the command. and append it to the end of the list.
            parsingPointer = parsingPointer->nextCommand; //move the parsing pointer to point to the newly created instance.
            parsingPointer->nextCommand = NULL;//Ensure that the end of the list is null.
        }
    }
    parsingPointer->command = command; //Assign the new incoming command to the command list instance.
    parsingPointer->commandCallback = commandCallback; //Set the callback function for the command.
    parsingPointer->numberOfArgumentsExpected = numberOfArguments; //Set the number of arguments that the command expects.
    parsingPointer->dataDelimiter = dataDelimiter; //Set the dataDelimiter
}

int BoltIoT::handleCommand() {
    CommandList *parsingPointer;//Declare a temporary pointer to parse through the list.
    int numberOfCommandsExecuted=0; //Initialize the number of commands executed to 0
    if(this->commandList==NULL){ //Check if any commands are configured.
        return numberOfCommandsExecuted; //If no command is configured, return 0
    }
    while (this->communicationPort->available() > 0) {  //Check if new data is available.
        char data = this->communicationPort->read(); //Read 1 character from the communication port.
        if (this->currentMonitoredCommand != NULL) { //Check if a command has been received and is the library is waiting for its arguments.
            bool runCommandSuccessful = this->runCommand(*(this->currentMonitoredCommand), data); //Try to execute the command. The command will run if the latest data ends the argument list.
            if (runCommandSuccessful) { //Check if the command was executed.
                this->communicationPort->println(this->currentMonitoredCommand->returnData); //If so send the data returned from the callback to the Bolt Device.
                this->currentMonitoredCommand = NULL; // Remove the instance from being monitored.
                numberOfCommandsExecuted++; //Increment the number of commands executed
            }
            continue;//Since the character that was pulled is already processed, the next character needs to be pulled.
        }
        this->receivedString += data; //If no command is waiting for arguments, add the received character to the received string.
        parsingPointer = this->commandList; //Point the temporary pointer to the start of the command list.
        while (parsingPointer != NULL) { // Check if the pointer has reached the end of the list. 
            if (isCommandReceived(parsingPointer->command)) { //Check if the command being pointed has been received.
                this->currentMonitoredCommand = parsingPointer;  //If the command is received, set it up to be monitored for arguments.
                if (this->currentMonitoredCommand->numberOfArgumentsExpected > 0) {  //Check if the command expects arguments.
                    this->currentMonitoredCommand->arguments = new String[this->currentMonitoredCommand->numberOfArgumentsExpected]; //If the command expects arguments, initialize a string array to hold the arguments.
                }
                this->currentMonitoredCommand->numberOfArgumentsReceived = this->DiscardCurrentData; //Set the number of arguments to -2, so that the runCommand function does not process the character passed along with the command. 
                bool runCommandSuccessful = this->runCommand(*(this->currentMonitoredCommand), this->currentMonitoredCommand->dataDelimiter); //Try to execute the command
                if (runCommandSuccessful) { //Check if the command is executed. The command executes here only if it does not expect any arguments.
                    this->communicationPort->println(this->currentMonitoredCommand->returnData);//If so send the data returned from the callback to the Bolt Device.
                    this->currentMonitoredCommand = NULL;// Remove the instance from being monitored.
                    numberOfCommandsExecuted++;//Increment the number of commands executed
                }
                this->receivedString = "";//Set the received string to be empty so that the command does not interfere with other commands.
                break;
            }
            parsingPointer = parsingPointer->nextCommand; //Move the temporary pointer to the next instance in the list.
        }
    }
    return numberOfCommandsExecuted; //Return the number of commands executed during the execution of this function.
}

bool BoltIoT::isCommandReceived(String command) {
    if (this->receivedString.endsWith(command)) { //Check if the received string ends with the command applied.
        return true; //If so return true to indicate that the command has been received.
    }
    return false; //If not return false to indicate that the command has not been received.
}

bool BoltIoT::runCommand(CommandList &commandSettings, char data) {
    if (commandSettings.numberOfArgumentsExpected > 0) {//Check if the command is expecting arguments.
        if (commandSettings.numberOfArgumentsReceived == this->DiscardCurrentData) { //If so confirm if the character data supplied is valid or not.
            commandSettings.numberOfArgumentsReceived = this->WaitForFirstDelimiter; //If the current data is not valid, set the command instance to wait for the first delimiter.
            return false; //Return false to indicate that the command was not executed.
        } else if (commandSettings.numberOfArgumentsReceived == this->WaitForFirstDelimiter) { //If not then check if the instance is waiting for the first delimiter.
            if (data == commandSettings.dataDelimiter) { //if so check if the delimiter has been received.
                commandSettings.numberOfArgumentsReceived = 0; //if the first delimiter has been received, start looking for the first argument.
            }
            return false; //Return false to indicate that the command was not executed.
        }
        if (data == commandSettings.dataDelimiter) {  //Check if the character received is a delimiter.
            commandSettings.numberOfArgumentsReceived++; //If so then increase the number of arguments received.
            if (commandSettings.numberOfArgumentsReceived < commandSettings.numberOfArgumentsExpected) { //Check the number of arguments received is less then the number of Arguments expected
                return false; //If so then return false to indicate that the command was not executed.
            }
            //If the control reaches here then the command will be executed. The only reason the command execution code is not here, is because less code lines can be used to take care of both cases of expecting arguments, and not expecting any arguments.
        } else { 
            commandSettings.arguments[commandSettings.numberOfArgumentsReceived] += data; //if the data received is not a delimiter, append it to the current argument.
            return false; //return false to indicate that the command was not executed.
        }
    }
    String returnValue = commandSettings.commandCallback(commandSettings.arguments); //Execute the callback function by sending the command arguments.
    if (returnValue.substring((returnValue.length() - 1)).equals("\n")) { //Check if the response from the callback ends with an newline.
        returnValue = returnValue.substring(0, returnValue.length() - 1); //If so then remove the newline.
    }
    commandSettings.returnData = returnValue; //Store the return value along with the command settings.
    return true; //return true to indicate that the command was executed.
}
