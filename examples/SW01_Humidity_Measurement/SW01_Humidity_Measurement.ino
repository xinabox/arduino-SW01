/*************************************************************
	This is an examples for the SW01
	Digital Temperature, Pressure and Humidity Sensor
	
	You can buy one on our store!
	-----> https://xinabox.cc/modules/sensors/SW01/
	
	This example request a humidity meausrement to be 
	made by the SW01 ad display the data over the Serial
	bus.
	
	The sensor communicates over the I2C Bus.
	
*************************************************************/

#include <XiX_Core.h>
#include <XiX_SW01.h>

const int DELAY_TIME 1000

void setup(){
	Serial.begin(9600);
	SW01.begin();
	Serial.println("================================");
	Serial.println("=    XINABOX SW01 Humidity     =");
	Serial.println("================================");	
	delay(DELAY_TIME);
}

void loop(){
	int value;
	value = 0;
	value = SW01.getHumidity();
	Serial.println("Humidity: ")
	Serial.print(value,2);
	Serial.print("%");
	delay(DELAY_TIME);
}