/*
	This is a library for the SW01 
	DIGITAL HUMIDITY, PRESSURE AND TEMPERATURE SENSOR

	The board uses I2C for communication.
	
	The board communicates with the following I2C device:
	-	BME280
	
	Data Sheets:
	BME280 - https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf
*/

#include "XiX_SW01.h"

bool XiX_SW01::begin(){
	xCore::write8(xCHIP, BME280_I2C_ADDRESS, BME280_REG_CONTROLHUMID, )
	xCore::write8(xCHIP, BME280_I2C_ADDRESS, BME280_REG_CONTROLMEASURE, )
	xCore::write8(xCHIP, BME280_I2C_ADDRESS, BME280_REG_CONFIG, ) 
} 

float XiX_SW01readPressure(void){
	
}

float XiX_SW01readAltitude(void){
	
}

float XiX_SW01readTemperature(void){
	
}




XiX_SW01 SW01 = XiX_SW01();