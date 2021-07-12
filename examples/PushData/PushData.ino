
#include <boltiot.h>

/*
 * This code monitors the status of pin 2, 
 * and sends the status fo the same to the cloud,
 * when the Bolt sends the RD\r command
 */

#ifndef API_KEY
#define API_KEY   "xxxxxxx-xxxx-xxxx-xxxx-xxxxxxxx"
#endif
#ifndef DEVICE_ID
#define DEVICE_ID "BOLTxxxxxxxx"
#endif

void setup(){
	boltiot.begin(Serial);
}

void loop(){
	boltiot.processPushDataCommand(digitalRead(2));
}
