/* Provided by Inventrom for interfacing bolt to Arduino */

#include <BoltIot-Arduino-Helper.h>

BoltIot boltiot;
void BoltIot::Begin(HardwareSerial& uart){
    this->communication_port=&uart;
    uart.begin(9600);
}

void BoltIot::Begin(int Rx_Pin,int Tx_Pin){
    this->communication_port=new SoftwareSerial(Rx_Pin,Tx_Pin);
    ((SoftwareSerial*)this->communication_port)->begin(9600);
    ((SoftwareSerial*)this->communication_port)->listen();
}


void BoltIot::CheckPoll(float a,float b,float c,float d,float e,float f){
    int length;
    if(this->communication_port->available()>0){
        char data=this->communication_port->read();
        this->command_received+=data;
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

String BoltIot::GetCommandString(){
    return this->command_received;
}
