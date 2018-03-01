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
        return false;
    }
}

bool BoltIoT::processPushDataCommand(float var1, float var2, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, "false");
    if (sendPushData) {
        this->sendVar(false, var2, isLastVar);
        return true;
    }
    return false;
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, var2, "false");
    if (sendPushData) {
        this->sendVar(false, var3, isLastVar);
        return true;
    }
    return false;
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, var2, var3, "false");
    if (sendPushData) {
        this->sendVar(false, var4, isLastVar);
        return true;
    }
    return false;
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, float var5, String isLastVar) {
    bool sendPushData = this->processPushDataCommand(var1, var2, var3, var4, "false");
    if (sendPushData) {
        this->sendVar(false, var5, isLastVar);
        return true;
    }
    return false;
}

bool BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, float var5, float var6) {
    bool sendPushData = this->processPushDataCommand(var1, var2, var3, var4, var5, "false");
    if (sendPushData) {
        this->sendVar(false, var6, "true");
        return true;
    }
    return false;
}

void BoltIoT::processPushDataCommand(float var1, float var2, float var3, float var4, float var5, float var6) {
    this->processPushDataCommand(var1, var2, var3, var4, var5, var6);
}

String BoltIoT::getReceivedString() {
    return this->receivedString;
}

void BoltIoT::setCommandString(String command, String(*commandCallback)(String*), int numberOfArguments, char dataDelimiter) {
    /*
     * This function is called to parse possible functions to be received from the Bolt.
     * The command string defines the possible function.
     * the command function points to a function which should be called in the event command is received.
     * number of arguments is optional with 0 default. The are the number of arguments, which BoltIoT should wait for before calling the command fucntion.
     * The arguments received are passed to the command function as an array of strings.
     */
    CommandList *temporaryPointer;
    temporaryPointer = commandList;
    if (commandList == NULL) {
        commandList = new CommandList();
        temporaryPointer = commandList;
        temporaryPointer->nextCommand = NULL;
    } else {
        while ((temporaryPointer->nextCommand != NULL)&&(!temporaryPointer->command.equals(command))) {
            temporaryPointer = temporaryPointer->nextCommand;
        }
        if (!temporaryPointer->command.equals(command)) {
            temporaryPointer->nextCommand = new CommandList();
            temporaryPointer = temporaryPointer->nextCommand;
            temporaryPointer->nextCommand = NULL;
        }
    }
    temporaryPointer->command = command;
    temporaryPointer->commandCallback = commandCallback;
    temporaryPointer->numberOfArguments = numberOfArguments;
    temporaryPointer->dataDelimiter = dataDelimiter;
}

void BoltIoT::handleCommand() {
    /*
     * This function replaces processPushDataCommand. and should be called periodically.
     */
    CommandList *tempararyPointer;

    while (this->communicationPort->available() > 0) {
        char data = this->communicationPort->read();
        if (this->currentMonitoredCommand != NULL) {
            bool runCommandSuccessful = this->runCommand(*(this->currentMonitoredCommand), data);
            if (runCommandSuccessful) {
                this->communicationPort->println(this->currentMonitoredCommand->returnData);
                this->currentMonitoredCommand = NULL;
            }
            continue;
        }
        this->receivedString += data;
        tempararyPointer = this->commandList;
        while ((tempararyPointer != NULL)&&(this->currentMonitoredCommand == NULL)) {
            if (isCommandReceived(tempararyPointer->command)) {
                this->currentMonitoredCommand = tempararyPointer;
                if (this->currentMonitoredCommand->numberOfArguments > 0) {
                    this->currentMonitoredCommand->arguments = new String[this->currentMonitoredCommand->numberOfArguments];
                }
                this->currentMonitoredCommand->numberOfArgumentsFound = -2;
                bool runCommandSuccessful = this->runCommand(*(this->currentMonitoredCommand), this->currentMonitoredCommand->dataDelimiter);
                if (runCommandSuccessful) {
                    this->communicationPort->println(this->currentMonitoredCommand->returnData);
                    this->currentMonitoredCommand = NULL;
                }
                this->receivedString = "";
                break;
            }
            tempararyPointer = tempararyPointer->nextCommand;
        }
    }
}

bool BoltIoT::isCommandReceived(String command) {
    if (this->receivedString.endsWith(command)) {
        return true;
    }
    return false;
}

bool BoltIoT::runCommand(CommandList &commandSettings, char data) {
    if (commandSettings.numberOfArguments > 0) {
        if (commandSettings.numberOfArgumentsFound == -2) {
            commandSettings.numberOfArgumentsFound = -1;
            return false;
        } else if (commandSettings.numberOfArgumentsFound == -1) {
            if (data == commandSettings.dataDelimiter) {
                commandSettings.numberOfArgumentsFound = 0;
            }
            return false;
        }
        if (data == commandSettings.dataDelimiter) {
            commandSettings.numberOfArgumentsFound++;
            if (commandSettings.numberOfArgumentsFound != commandSettings.numberOfArguments) {
                return false;
            }
        } else {
            commandSettings.arguments[commandSettings.numberOfArgumentsFound] += data;
            return false;
        }
    }
    String returnValue = commandSettings.commandCallback(commandSettings.arguments);
    if (returnValue.substring((returnValue.length() - 1)).equals("\n")) {
        returnValue = returnValue.substring(0, returnValue.length() - 1);
    }
    commandSettings.returnData = returnValue;
    return true;
}