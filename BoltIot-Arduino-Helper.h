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

/* Class which handles the communication with bolt over UART
 * The UART can be any hardware serial or software serial port
 */
class BoltIot{
    Stream *communication_port; //Holds the communication port at which Bolt is connected.
    String command_received;    //Holds the command currently being passed
public:
    /* Used to initialize the bolt class with Hardware Serial port. The function starts the uart at 9600, the required baud rate for bolt
     * uart-Hardware serial port to which Bolt is connected. Can be Serial, Serial0, Serial1
     */
    void Begin(HardwareSerial& uart);


    /* Used to initialize the bolt class with Software Serial port. The function starts the software uart at 9600, the required baud rate for bolt
     * Rx_Pin- The arduino pin number to which the Tx pin of Bolt is connected.
     * Tx_Pin- The arduino pin number to which the Rx pin of Bolt is connected.
     */
    void Begin(int Rx_Pin,int Tx_Pin);


    /* This function has to be called from the loop function.
     * The CheckPoll function checks for data received on the uart, and confirms with the bolt has sent the read command("RD\r") to the Arduino 
     * If the arduino has received the read command from the bolt, the functions sends back 6 float csv's, which are provided as the variables
     * At the least 1 variable has to be set by the user.
     */
    void CheckPoll(float a,float b=0,float c=0,float d=0,float e=0,float f=0);


    /* @Future_Development:
     * This string currently only provides the data buffered in the bolt class.
     */
    String GetCommandString();
};

extern BoltIot boltiot; //Variable defined in the header so that user does not have to define the variable
#endif /* BOLT_H */

