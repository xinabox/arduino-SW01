/*
	This is a library for the SW01
	DIGITAL HUMIDITY, PRESSURE AND TEMPERATURE SENSOR

	The board uses I2C for communication.
	
	The board communicates with the following I2C device:
	- 	BME280
	
	Data Sheets:
	BME280 - https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf
*/

#ifndef XiX_SW01_h
#define XiX_SW01_h

// System Includes
#include <inttypes.h>
#include <Arduino.h>
#include "XiX_Core.h"

//Device I2C Address
#define BME280_I2C_ADDRESS			0x77

// Register Defines
#define BME280_REG_SOFTRESET   		0xE0

#define BME280_REG_CAL26     		0xE1

#define BME280_REG_CONTROLHUMID 	0xF2
#define BME280_REG_STATUS       	0XF3
#define BME280_REG_CONTROLMEASURE   0xF4
#define BME280_REG_CONFIG         	0xF5
#define BME280_REG_PRESSURE  		0xF7
#define BME280_REG_TEMP      		0xFA
#define BME280_REG_HUMID    		0xFD
#define BME280_TEMP_DIG_ADDR  		0x88
#define BME280_PRESS_DIG_ADDR 		0x8E
#define BME280_HUM_DIG_ADDR1  		0xA1
#define BME280_HUM_DIG_ADDR2  		0xE1

// Oversampling Defines
#define BME_OSR_1	0x01
#define BME_OSR_2	0x02
#define BME_OSR_4	0x03
#define BME_OSR_8	0x04

// Device ID
#define BME280_REG_CHIPID      		0xD0
#define BME280_DEVICE_ID         	0x60

/*=========================================================================*/

class XiX_SW01: public xCore
{
	protected:		
		bool readTrim();
		bool readData();
		float calc_temp();
		float calc_himid();
    public:
		XiX_SW01(){}
		bool begin();
		void setMode();
		float readPressure(void);
		float readAltitude(void);
		float readTemperature(void);
	private:
		uint8_t mode;
}

etern XiX_SW01 SW01;

#endif