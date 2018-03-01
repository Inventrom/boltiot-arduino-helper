/* Provided by Inventrom for interfacing bolt to Arduino */
#include <Arduino.h>
#include "BoltIoT-Arduino-Helper.h"

BoltIoT boltiot;

BoltIoT::BoltIoT() {
    this->commandList = NULL;
}

void BoltIoT::Begin(HardwareSerial& uart){
    this->begin(uart);
}

void BoltIoT::Begin(int Rx_Pin, int Tx_Pin){
    this->begin(Rx_Pin,Tx_Pin);
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

void BoltIoT::sendVar(float var, String sendNewLine){
    this->communicationPort->print(","+String(var));
    if(sendNewLine.equals("true")){
        this->communicationPort->print("\n");
    }
}

bool BoltIoT::checkPoll(float var1, String sendNewLine) {
    int length;
    if (this->communicationPort->available() > 0) {
        char data = this->communicationPort->read();
        this->receivedString += data;
        if (this->receivedString.equals("RD\r")) {
            this->communicationPort->print(String(var1));
            if (sendNewLine.equals("true")) {
                this->communicationPort->print("\n");
            }
            return true;
        }
        return false;
    }
}

bool BoltIoT::checkPoll(float var1, float var2, String sendNewLine) {
    bool sendPushData = this->checkPoll(var1, "false");
    if (sendPushData) {
        this->sendVar(var2, sendNewLine);
        return true;
    }
    return false;
}

bool BoltIoT::checkPoll(float var1, float var2, float var3, String sendNewLine) {
    bool sendPushData = this->checkPoll(var1, var2, "false");
    if (sendPushData) {
        this->sendVar(var3, sendNewLine);
        return true;
    }
    return false;
}

bool BoltIoT::checkPoll(float var1, float var2, float var3, float var4, String sendNewLine) {
    bool sendPushData = this->checkPoll(var1, var2, var3, "false");
    if (sendPushData) {
        this->sendVar(var4, sendNewLine);
        return true;
    }
    return false;
}

bool BoltIoT::checkPoll(float var1, float var2, float var3, float var4, float var5, String sendNewLine) {
    bool sendPushData = this->checkPoll(var1, var2, var3, var4, "false");
    if (sendPushData) {
        this->sendVar(var5, sendNewLine);
        return true;
    }
    return false;
}

bool BoltIoT::checkPoll(float var1, float var2, float var3, float var4, float var5, float var6) {
    bool sendPushData = this->checkPoll(var1, var2, var3, var4, var5, "false");
    if (sendPushData) {
        this->sendVar(var6, "true");
        return true;
    }
    return false;
}

void BoltIoT::CheckPoll(float var1, float var2, float var3, float var4, float var5, float var6) {
    this->CheckPoll(var1, var2, var3, var4, var5, var6);
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
     * This function replaces checkpoll. and should be called periodically.
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
    int length = this->receivedString.length() - command.length();
    if (length < 0) {
        return false;
    }
    if (command.equals(this->receivedString.substring(length))) {
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