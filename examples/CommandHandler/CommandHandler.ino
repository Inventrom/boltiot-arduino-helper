
#include <BoltIoT-Arduino-Helper.h>


String Return_Loggable_Data(String *data){
	String retval="";
	retval=retval+analogRead(A0)+",";
	retval=retval+analogRead(A1)+",";
	retval=retval+analogRead(A2)+",";
	retval=retval+analogRead(A3)+",";
	retval=retval+analogRead(A4)+",";
	retval=retval+analogRead(A5);
	return retval;
}


String Return_Extra_Monitoring_Data(String *data){
	String retval="";
	retval=retval+digitalRead(2)+",";
	retval=retval+digitalRead(3)+",";
	retval=retval+digitalRead(4)+",";
	retval=retval+digitalRead(5)+",";
	retval=retval+digitalRead(6);
	return retval;	
}

String Run_PWM_Command(String *data){
	int PWM_Channel=data[0].toInt();
	int PWM_Value=data[1].toInt();
}

void setup(){
	boltiot.Begin(Serial);
	boltiot.SetCommandString("RD\r",Return_Loggable_Data);
	boltiot.SetCommandString("GetData",Return_Extra_Monitoring_Data);
	boltiot.SetCommandString("PWM",Run_PWM_Command,2);  //2 arguments are required, 1 is pwm channel and 2 is pwm value.
}

void loop(){
	boltiot.HandleCommand();
}
