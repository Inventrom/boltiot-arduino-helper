/* Provided by Inventrom for interfacing bolt to Arduino */
#include <Arduino.h>
#include "BoltIot-Arduino-Helper.h"

BoltIot boltiot;

BoltIot::BoltIot() {
    this->commandList = NULL;
    this->argumentReadTimeout = -1;
}

void BoltIot::Begin(HardwareSerial& uart) {
    this->communication_port = &uart;
    uart.begin(9600);
}

void BoltIot::Begin(int Rx_Pin, int Tx_Pin) {
    this->communication_port = new SoftwareSerial(Rx_Pin, Tx_Pin);
    ((SoftwareSerial*)this->communication_port)->begin(9600);
    ((SoftwareSerial*)this->communication_port)->listen();
}

void BoltIot::CheckPoll(float a, float b, float c, float d, float e, float f) {
    int length;
    if (this->communication_port->available() > 0) {
        char data = this->communication_port->read();
        this->command_received += data;
        if (this->command_received.equals("RD\r")) {
            this->communication_port->print(a);
            this->communication_port->print(",");
            this->communication_port->print(b);
            this->communication_port->print(",");
            this->communication_port->print(c);
            this->communication_port->print(",");
            this->communication_port->print(d);
            this->communication_port->print(",");
            this->communication_port->print(e);
            this->communication_port->print(",");
            this->communication_port->println(f);
            this->command_received = "";
        }
        length = this->command_received.length();
        if (length > 2) {
            this->command_received = this->command_received.substring(length - 2, length);
        }
    }
}

String BoltIot::GetCommandString() {
    return this->command_received;
}

void BoltIot::SetCommandString(String command, String(*command_function)(String*), int number_of_arguments, char dataDelimiter) {
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
        temp->next = NULL;
    } else {
        while ((temp->next != NULL)&&(!temp->command.equals(command))) {
            temp = temp->next;
        }
        if (!temp->command.equals(command)) {
            temp->next = new CommandList();
            temp = temp->next;
            temp->next = NULL;
        }
    }
    temp->command = command;
    temp->command_function = command_function;
    temp->number_of_arguments = number_of_arguments;
    temp->dataDelimiter = dataDelimiter;
}

void BoltIot::HandleCommand() {
    /*
     * This function replaces checkpoll. and should be called periodically.
     */
    CommandList *temp;

    while (this->communication_port->available() > 0) {
        char data = this->communication_port->read();
        if (this->currentMonitoredCommand != NULL) {
            bool runCommandSuccessful = this->runCommand(*(this->currentMonitoredCommand), data);
            if (runCommandSuccessful) {
                this->communication_port->println(this->currentMonitoredCommand->returnData);
                this->currentMonitoredCommand = NULL;
            }
            continue;
        }
        this->command_received += data;
        temp = this->commandList;
        while ((temp != NULL)&&(this->currentMonitoredCommand == NULL)) {
            if (checkIfCommandReceived(temp->command)) {
                this->currentMonitoredCommand = temp;
                if (this->currentMonitoredCommand->number_of_arguments > 0) {
                    this->currentMonitoredCommand->arguments = new String[this->currentMonitoredCommand->number_of_arguments];
                }
                this->currentMonitoredCommand->numberOfArgumentsFound = -2;
                bool runCommandSuccessful = this->runCommand(*(this->currentMonitoredCommand), this->currentMonitoredCommand->dataDelimiter);
                if (runCommandSuccessful) {
                    this->communication_port->println(this->currentMonitoredCommand->returnData);
                    this->currentMonitoredCommand = NULL;
                }
                this->command_received = "";
                break;
            }
            temp = temp->next;
        }
    }
}

bool BoltIot::checkIfCommandReceived(String command) {
    int length = this->command_received.length() - command.length();
    if (length < 0) {
        return false;
    }
    if (command.equals(this->command_received.substring(length))) {
        return true;
    }
    return false;
}

bool BoltIot::runCommand(CommandList &commandStruct, char data) {
    if (commandStruct.number_of_arguments > 0) {
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
            if (commandStruct.numberOfArgumentsFound != commandStruct.number_of_arguments) {
                return false;
            }
        } else {
            commandStruct.arguments[commandStruct.numberOfArgumentsFound] += data;
            return false;
        }
    }
    String returnValue = commandStruct.command_function(commandStruct.arguments);
    if (returnValue.substring((returnValue.length() - 1)).equals("\n")) {
        returnValue = returnValue.substring(0, returnValue.length() - 1);
    }
    commandStruct.returnData = returnValue;
    return true;
}