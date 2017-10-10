/*
	This is a library for the SW01 
	DIGITAL HUMIDITY, PRESSURE AND TEMPERATURE SENSOR

	The board uses I2C for communication.
	
	The board communicates with the following I2C device:
	-	BME280
	
	Data Sheets:
	BME280 - https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf
*/

#include "xSW01.h"
#include <math.h>
  
/*--Public Class Function--*/

/********************************************************
 	Constructor
*********************************************************/
xSW01::xSW01(void)
{
	tempcal = 0.0;
	temperature = 0.0;
	humidity = 0.0;
	pressure = 0.0;
	altitude = 0.0;
	dewpoint = 0.0;
}

/********************************************************
 	Configure Sensor
*********************************************************/
bool xSW01::begin(void)
{
	readSensorCoefficients();
	xCore.write8(BME280_I2C_ADDRESS, BME280_REG_CONTROLHUMID, 0x01);
	xCore.write8(BME280_I2C_ADDRESS, BME280_REG_CONTROLMEASURE, 0x3F);
	return true;
} 

/********************************************************
 	Read Data from BME280 Sensor
*********************************************************/
void xSW01::poll(void)
{
	readTemperature();
	readHumidity();
	readPressure();
}

/********************************************************
 	Read Pressure from BME280 Sensor in Pa
*********************************************************/
float xSW01::getPressure(void)
{
	return pressure;
}

/********************************************************
 	Read Altitude from BME280 Sensor in meters
*********************************************************/
float xSW01::getAltitude(void)
{
	float atmospheric = pressure / 100.0;
	altitude = 44330.0 * (1.0 - pow((atmospheric/1013.25), 1/5.255));
	return altitude;
}

/********************************************************
 	Temperature from BME280 Sensor in Celcuis
*********************************************************/
float xSW01::getTempC(void)
{
    temperature = temperature + tempcal;
    return temperature;	
}

/********************************************************
 	Convert Temperature from BME280 Sensor to Farenhied
*********************************************************/
float xSW01::getTempF(void)
{
	temperature = temperature + tempcal;
  return temperature * 1.8 + 32;	
}

/********************************************************
 	Read Humidity from BME280 Sensor 
*********************************************************/
float xSW01::getHumidity(void)
{
	return humidity;
}

/********************************************************
 	Set temperature calibration data
*********************************************************/
void xSW01::setTempCal(float offset)
{
	tempcal = offset;
}


/********************************************************
 	Read Dew Point from BME280 Sensor in Celcuis
*********************************************************/
float xSW01::getDewPoint(void)
{
    dewpoint = 243.04 * (log(humidity/100.0) + ((17.625 * temperature)/(243.04 + temperature)))
    /(17.625 - log(humidity/100.0) - ((17.625 * temperature)/(243.04 + temperature)));
	
	return dewpoint;
}

/*--Private Class Function--*/

/********************************************************
 	Read Temperature from BME280 Sensor 
*********************************************************/
void xSW01::readTemperature(void)
{
    int32_t var1, var2;
    
    int32_t rawTemp = ((uint32_t)xCore.read8(BME280_I2C_ADDRESS, BME280_REG_TEMP_MSB) << 12);
	rawTemp |= ((uint32_t)xCore.read8(BME280_I2C_ADDRESS, BME280_REG_TEMP_MSB) << 4);
	rawTemp |= ((xCore.read8(BME280_I2C_ADDRESS, BME280_REG_TEMP_MSB) << 4) & 0x0F);
  
    
    var1  = ((((rawTemp >>3) - ((int32_t)cal_data.dig_T1 <<1)))*((int32_t)cal_data.dig_T2)) >> 11;
    
    var2  = (((((rawTemp >>4) - ((int32_t)cal_data.dig_T1))*((rawTemp >>4) - ((int32_t)cal_data.dig_T1))) >> 12)*((int32_t)cal_data.dig_T3)) >> 14;
    
    t_fine = var1 + var2;    
    
    temperature  = (t_fine * 5 + 128) >> 8;
    
    temperature = temperature / 100;
}

/********************************************************
 	Read Pressure from BME280 Sensor 
*********************************************************/
void xSW01::readPressure(void)
{
	int64_t var1, var2, p;
    
    int32_t rawPressure = xCore.read24(BME280_I2C_ADDRESS, BME280_REG_PRESSURE);    
    rawPressure >>= 4;
    
    var1 = 	((int64_t)t_fine) - 128000;
    
    var2 = 	var1 * var1 * (int64_t)cal_data.dig_P6;
	
    var2 = 	var2 + ((var1*(int64_t)cal_data.dig_P5)<<17); 
	
    var2 = 	var2 + (((int64_t)cal_data.dig_P4)<<35);
	
    var1 = 	((var1 * var1 * (int64_t)cal_data.dig_P3)>>8) + ((var1 * (int64_t)cal_data.dig_P2)<<12);
    
	var1 = 	(((((int64_t)1)<<47)+var1))*((int64_t)cal_data.dig_P1)>>33;
   
    if (var1 == 0) {
        pressure = 0.0;
    }
    
    p = 1048576 - rawPressure;
    p = (((p<<31) - var2)*3125) / var1;  
    var1 = (((int64_t)cal_data.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)cal_data.dig_P8) * p) >> 19;
    
    p = ((p + var1 + var2) >> 8) + (((int64_t)cal_data.dig_P7)<<4);
    
    pressure = (float)p/256;
}

/********************************************************
 	Read Humidity from BME280 Sensor 
*********************************************************/
void xSW01::readHumidity(void)
{
	    
    int32_t rawHumidity = xCore.read16(BME280_I2C_ADDRESS, BME280_REG_HUMID);
    
    int32_t v_x1_u32r;
    
    v_x1_u32r = (t_fine - ((int32_t)76800));
	
    v_x1_u32r = (((((rawHumidity << 14) - (((int32_t)cal_data.dig_H4) << 20) - 
				(((int32_t)cal_data.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *  
				(((((((v_x1_u32r * ((int32_t)cal_data.dig_H6)) >> 10) * (((v_x1_u32r * 
				((int32_t)cal_data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + 
				((int32_t)2097152)) * ((int32_t)cal_data.dig_H2) + 8192) >> 14));
    
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * 
				((int32_t)cal_data.dig_H1)) >> 4));
				
    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;  
	
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    
    float h = (v_x1_u32r>>12);
    
    humidity = h / 1024.0;
}

/********************************************************
 	Read Sensor Cailbration Data from BME280 Sensor 
*********************************************************/
void xSW01::readSensorCoefficients(void)
{
    cal_data.dig_T1 = xCore.read16_LE(BME280_I2C_ADDRESS, BME280_DIG_T1_REG);   
    cal_data.dig_T2 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_T2_REG);    
    cal_data.dig_T3 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_T3_REG);   
    cal_data.dig_P1 = xCore.read16_LE(BME280_I2C_ADDRESS, BME280_DIG_P1_REG);   
    cal_data.dig_P2 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P2_REG);    
    cal_data.dig_P3 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P3_REG);    
    cal_data.dig_P4 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P4_REG);   
    cal_data.dig_P5 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P5_REG);    
    cal_data.dig_P6 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P6_REG);    
    cal_data.dig_P7 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P7_REG);    
    cal_data.dig_P8 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P8_REG);    
    cal_data.dig_P9 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_P9_REG);    
    cal_data.dig_H1 = xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H1_REG);    
    cal_data.dig_H2 = xCore.readS16_LE(BME280_I2C_ADDRESS, BME280_DIG_H2_REG);    
    cal_data.dig_H3 = xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H3_REG);    
    cal_data.dig_H4 = (xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H4_REG) << 4) | (xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H4_REG+1) & 0xF);    
    cal_data.dig_H5 = (xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H5_REG+1) << 4) | (xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H5_REG) >> 4);    
    cal_data.dig_H6 = (int8_t)xCore.read8(BME280_I2C_ADDRESS, BME280_DIG_H6_REG);   
}

