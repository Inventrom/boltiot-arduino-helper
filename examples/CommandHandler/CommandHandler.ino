
#include <BoltIoT-Arduino-Helper.h>

/*
 * With this code the user will be able to use the SerialWrite API 
 * to send commands to the Boltduino.
 * The User will also be able to parse the response of the command 
 * using the SerialRead API
 */

String getAnalogData(String *data){
	String retval="";
	retval=retval+analogRead(A0)+",";
	retval=retval+analogRead(A1)+",";
	retval=retval+analogRead(A2)+",";
	retval=retval+analogRead(A3)+",";
	retval=retval+analogRead(A4)+",";
	retval=retval+analogRead(A5);
	return retval;
}


String getDigitalData(String *data){
	String retval="";
	retval=retval+digitalRead(2)+",";
	retval=retval+digitalRead(3)+",";
	retval=retval+digitalRead(4)+",";
	retval=retval+digitalRead(5)+",";
	retval=retval+digitalRead(6);
	return retval;	
}

String setDigitalPin(String *data){
  int pinNumber=data[0].toInt();
  int pinState=data[1].toInt()>0?1:0;
  digitalWrite(pinNumber,pinState);
  return "Pin "+String(pinNumber)+" set to Digital value "+String(pinState);
}

String runPwmCommand(String *data){
	int pwmChannel=data[0].toInt();
	int pwmValue=data[1].toInt();
  analogWrite(pwmChannel,pwmValue);
  return "Pin "+String(pwmChannel)+" set to PWM value "+String(pwmValue);
}

void setup(){
	boltiot.begin(Serial);
	boltiot.setCommandString("GetAnalogData",getAnalogData);
	boltiot.setCommandString("GetDigitalData",getDigitalData);
  boltiot.setCommandString("SetDigitalPin",setDigitalPin,2);  //2 arguments are required, argument 1 is pin number and 2 is digital value. The command and argument should be seperted by a ' ' character, ' ' is the default. example SetDigitalPin 6 0 . 
  boltiot.setCommandString("SetPWM",runPwmCommand,2,'.');  //2 arguments are required, argument 1 is pwm channel and 2 is pwm value. The command and argument should be seperted by a '.' character. example SetPWM.6.200. . 
}

void loop(){
	boltiot.handleCommand();
}
