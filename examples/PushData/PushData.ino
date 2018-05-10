
#include <BoltIoT-Arduino-Helper.h>

/*
 * This code monitors the status of pin 2, 
 * and sends the status fo the same to the cloud,
 * when the Bolt sends the RD\r command
 */



void setup(){
	boltiot.begin(Serial);
}

void loop(){
	boltiot.processPushDataCommand(digitalRead(2));
}
