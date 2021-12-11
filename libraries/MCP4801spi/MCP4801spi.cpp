/*
  MCP4801.cpp - Library for interacting with the Microchip MCP4801 
                8-Bit Voltage Output Digital-to-Analog Converter
                
 */

#include <SPI.h>
#include <MCP4801spi.h>

MCP4801::MCP4801( int csPin, int ldacPin )   {

  	_csPin = csPin;
 	 _ldacPin  = ldacPin;

 	 pinMode(_csPin, OUTPUT);
  	pinMode(_ldacPin, OUTPUT);
  
 	 digitalWrite(_csPin, HIGH);  //comm. off
  	digitalWrite(_ldacPin, HIGH);         //latch off
}


int MCP4801::setOutput(int value){

 	 int packet = 0;
    
  	packet = (value << 4) | GAIN_SHTDWN_BITS;  

  	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
 	 digitalWrite(_csPin, LOW);   //set chip as listener

  	SPI.transfer(highByte(packet));          //send packet
  	SPI.transfer(lowByte(packet));

  	digitalWrite(_csPin, HIGH);  //release chip select
	SPI.endTransaction();

 	 digitalWrite(_ldacPin, LOW);          //pull latch down
  	digitalWrite(_ldacPin, HIGH);         //pull latch up
  
}