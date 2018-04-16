
#include <BoltIoT-Arduino-Helper.h>


String returnLoggableData(String *data){
	String retval="";
	retval=retval+analogRead(A0)+",";
	retval=retval+analogRead(A1)+",";
	retval=retval+analogRead(A2)+",";
	retval=retval+analogRead(A3)+",";
	retval=retval+analogRead(A4)+",";
	retval=retval+analogRead(A5);
	return retval;
}


String returnExtraMonitoringData(String *data){
	String retval="";
	retval=retval+digitalRead(2)+",";
	retval=retval+digitalRead(3)+",";
	retval=retval+digitalRead(4)+",";
	retval=retval+digitalRead(5)+",";
	retval=retval+digitalRead(6);
	return retval;	
}

String runPwmCommand(String *data){
	int pwmChannel=data[0].toInt();
	int pwmValue=data[1].toInt();
}

String testReturn(String data[]){
  return "This works fine";
}

void setup(){
	boltiot.begin(Serial);
  boltiot.setCommandString("RD\r",returnLoggableData);
  boltiot.setCommandString("Test Return",testReturn);
	boltiot.setCommandString("GetData",returnExtraMonitoringData);
	boltiot.setCommandString("PWM",runPwmCommand,2);  //2 arguments are required, 1 is pwm channel and 2 is pwm value.
}

void loop(){
	boltiot.handleCommand();
}
