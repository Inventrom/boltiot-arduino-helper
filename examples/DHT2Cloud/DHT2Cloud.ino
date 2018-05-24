
#include <BoltIoT-Arduino-Helper.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Depends on the following Arduino library:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
/*
 * This code combines Push data code with Command Handler Code.
 * With this code the user will be able to use the SerialWrite API 
 * to send commands to the Boltduino.
 * The User will also be able to parse the response of the command 
 * using the SerialRead API
 * 
 * Additionally, when the Boltduino received RD\r from the Bolt,
 * It will send the status of pin 2 to the Bolt, to be logged to the cloud.
 */

#define DHTPIN            2

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11 
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

float Temperature,Humidity;
bool TemperatureValueIsValid=false;
bool HumidityValueIsValid=false;

long NextMeasurementTime;

uint32_t delayMS;
String pushData(String *data){
  return String(Temperature)+","
  +String(Humidity)+","
  +String(TemperatureValueIsValid)+","
  +String(HumidityValueIsValid);
}

String getSensorDetail(String *data){
  String retval="",units="";
  sensor_t sensor;
  if(data[0].equals("Temperature")){
    dht.temperature().getSensor(&sensor);
    units="*C";
  }else if(data[0].equals("Humidity")){
    dht.humidity().getSensor(&sensor);
    units="%";
  }else{
    return "Invalid Sensor Type";
  }
  if(data[1].equals("Sensor")){
    retval=String(sensor.name);
  }else if(data[1].equals("Driver Ver")){
    retval=String(sensor.version);
  }else if(data[1].equals("Unique ID")){
    retval=String(sensor.sensor_id);
  }else if(data[1].equals("Max Value")){
    retval=String(sensor.max_value)+units;
  }else if(data[1].equals("Min Value")){
    retval=String(sensor.min_value)+units;
  }else if(data[1].equals("Resolution")){
    retval=String(sensor.resolution)+units;
  }else{
    retval="Unknown parameter";
  }
  return retval;
}

void setup(){
  boltiot.begin(Serial);
  dht.begin();
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);
  NextMeasurementTime=millis()+delayMS;
  boltiot.setCommandString("RD\r",pushData);
  boltiot.setCommandString("GetData",pushData);
  boltiot.setCommandString("GetSensorDetails",getSensorDetail,2,'.'); //2 arguments are required. The command and argument should be seperted by a '.' character.
}

void loop(){
  boltiot.handleCommand();
  if(NextMeasurementTime>millis()){
    return;
  }
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature)){
    TemperatureValueIsValid=false;
  }else{
    TemperatureValueIsValid=true;
    Temperature=event.temperature;
  }
  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity)){
    HumidityValueIsValid=false;
  }else{
    HumidityValueIsValid=true;
    Humidity=event.relative_humidity;
  }
}
