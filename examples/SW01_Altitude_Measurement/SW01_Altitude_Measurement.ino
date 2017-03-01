/*************************************************************
	This is an examples for the SW01
	Digital Temperature, Pressure and Humidity Sensor
	
	You can buy one on our store!
	-----> https://xinabox.cc/SW01/
	
	This example request a altitude meausrement to be 
	made by the SW01 and display the data over the Serial
	bus.
	
	Currently Supported on the following ☒CHIP:
	- CW01
	- CC01
	- CR01
	
	The sensor communicates over the I2C Bus.
	
	------------------------TIPS--------------------------
	Change this line ----->Wire.begin(2,14);
	to this			 ----->Wire.begin();
	to allow this sensor to communicate with CC01 and CR01
	
*************************************************************/

#include <xCore.h>
#include <xSW01.h>

const int DELAY_TIME = 1000;

void setup(){
	// Start the Serial Monitor
	Serial.begin(115200);
	
	// Start the I2C Comunication
	Wire.begin(2,14);
	
	// Start the  SW01 Sensor
	SW01.begin();
	
	Serial.println("================================");
	Serial.println("     XINABOX SW01 Altitude      ");
	Serial.println("================================");	
	delay(DELAY_TIME);
}

void loop(){
	// Create a variable to store the data read from SW0
	float alt;
	alt = 0;

  // Read and calculate data from SW01 sensor
  SW01.poll();
  
	// Request SW01 to get the altitude measurement and store in
	// the altitude variable	
	alt = SW01.getAltitude();
	
	// Display the recoreded data over the Serial Monitor	
	Serial.print("Altitude: ");
	Serial.print(alt);
	Serial.println(" m");
	
	// Small delay between sensor reads
	delay(DELAY_TIME);
}
