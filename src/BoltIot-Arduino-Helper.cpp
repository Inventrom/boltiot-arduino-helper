/* Provided by Inventrom for interfacing bolt to Arduino */
#include <Arduino.h>
#include "BoltIot-Arduino-Helper.h"

BoltIoT boltiot;

BoltIoT::BoltIoT() {
    this->commandList = NULL;
    this->argumentReadTimeout = -1;
}

void BoltIoT::begin(HardwareSerial& uart) {
    this->communicationPort = &uart;
    uart.begin(9600);
}

void BoltIoT::begin(int Rx_Pin, int Tx_Pin) {
    this->communicationPort = new SoftwareSerial(Rx_Pin, Tx_Pin);
    ((SoftwareSerial*)this->communicationPort)->begin(9600);
    ((SoftwareSerial*)this->communicationPort)->listen();
}

void BoltIoT::CheckPoll(float a, float b, float c, float d, float e, float f) {
    int length;
    if (this->communicationPort->available() > 0) {
        char data = this->communicationPort->read();
        this->receivedString += data;
        if (this->receivedString.equals("RD\r")) {
            this->communicationPort->print(a);
            this->communicationPort->print(",");
            this->communicationPort->print(b);
            this->communicationPort->print(",");
            this->communicationPort->print(c);
            this->communicationPort->print(",");
            this->communicationPort->print(d);
            this->communicationPort->print(",");
            this->communicationPort->print(e);
            this->communicationPort->print(",");
            this->communicationPort->println(f);
            this->receivedString = "";
        }
        length = this->receivedString.length();
        if (length > 2) {
            this->receivedString = this->receivedString.substring(length - 2, length);
        }
    }
}

String BoltIoT::getReceivedString() {
    return this->receivedString;
}

void BoltIoT::setCommandString(String command, String(*command_function)(String*), int number_of_arguments, char dataDelimiter) {
    /*
     * This function is called to parse possible functions to be received from the Bolt.
     * The command string defines the possible function.
     * the command function points to a function which should be called in the event command is received.
     * number of arguments is optional with 0 default. The are the number of arguments, which BoltIoT should wait for before calling the command fucntion.
     * The arguments received are passed to the command function as an array of strings.
     */
    CommandList *temp;
    temp = commandList;
    if (commandList == NULL) {
        commandList = new CommandList();
        temp = commandList;
        temp->nextCommand = NULL;
    } else {
        while ((temp->nextCommand != NULL)&&(!temp->command.equals(command))) {
            temp = temp->nextCommand;
        }
        if (!temp->command.equals(command)) {
            temp->nextCommand = new CommandList();
            temp = temp->nextCommand;
            temp->nextCommand = NULL;
        }
    }
    temp->command = command;
    temp->commandCallback = command_function;
    temp->numberOfArguments = number_of_arguments;
    temp->dataDelimiter = dataDelimiter;
}

void BoltIoT::handleCommand() {
    /*
     * This function replaces checkpoll. and should be called periodically.
     */
    CommandList *temp;

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
        temp = this->commandList;
        while ((temp != NULL)&&(this->currentMonitoredCommand == NULL)) {
            if (isCommandReceived(temp->command)) {
                this->currentMonitoredCommand = temp;
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
            temp = temp->nextCommand;
        }
    }
}

bool BoltIoT::isCommandReceived(String command) {
    int length = this->receivedString.length() - command.length();
    if (length < 0) {
        return false;
    }
    if (command.equals(this->receivedString.substring(length))) {
        return true;
    }
    return false;
}

bool BoltIoT::runCommand(CommandList &commandStruct, char data) {
    if (commandStruct.numberOfArguments > 0) {
        if (commandStruct.numberOfArgumentsFound == -2) {
            commandStruct.numberOfArgumentsFound = -1;
            return false;
        } else if (commandStruct.numberOfArgumentsFound == -1) {
            if (data == commandStruct.dataDelimiter) {
                commandStruct.numberOfArgumentsFound = 0;
            }
            return false;
        }
        if (data == commandStruct.dataDelimiter) {
            commandStruct.numberOfArgumentsFound++;
            if (commandStruct.numberOfArgumentsFound != commandStruct.numberOfArguments) {
                return false;
            }
        } else {
            commandStruct.arguments[commandStruct.numberOfArgumentsFound] += data;
            return false;
        }
    }
    String returnValue = commandStruct.commandCallback(commandStruct.arguments);
    if (returnValue.substring((returnValue.length() - 1)).equals("\n")) {
        returnValue = returnValue.substring(0, returnValue.length() - 1);
    }
    commandStruct.returnData = returnValue;
    return true;
}