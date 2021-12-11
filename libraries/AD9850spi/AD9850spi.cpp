/**************************************************************************/
/*! 
    @file     AD9850.cpp
    @author   Louis J Haskell
	@license  BSD 
	
	Driver for AD9850 DDS frequency generator board

	@section  HISTORY

    v1.0 - First release
     v1.1  Generalized pin assignments so any pin numbers can be used
*/
/**************************************************************************/

#include "Arduino.h"
#include <SPI.h>
#include  <AD9850spi.h>


AD9850::AD9850( int in_fqud, int in_reset ) {
	
	fqud = in_fqud;
	reset = in_reset;
	pinMode(fqud, OUTPUT);
	pinMode(reset, OUTPUT); 

	digitalWrite(reset, HIGH); digitalWrite(reset, LOW); 

	digitalWrite(fqud, LOW); 
	digitalWrite(13, HIGH); digitalWrite(13, LOW); 
	digitalWrite(fqud, HIGH); 


}

void AD9850::setSerialMode() {

	// this pulse enables serial mode - Datasheet page 12 figure 10
	digitalWrite(fqud, LOW); 
	digitalWrite(13, HIGH); digitalWrite(13, LOW); 
	digitalWrite(fqud, HIGH); 

}

void AD9850::setFrequency( int32_t frequency )
{
  
  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850
  
	digitalWrite(fqud, LOW); 
	SPI.beginTransaction(SPISettings(14000000, LSBFIRST, SPI_MODE0));
	SPI.transfer( (byte) freq );
	SPI.transfer( (byte) (freq >> 8 ) );
	SPI.transfer( (byte) (freq >> 16) );
	SPI.transfer( (byte) (freq >> 24 ) );
	SPI.transfer( 0);
   
  	digitalWrite(fqud, HIGH); 

	// This is a hack. Put the bit order back to MSB for the LCD display
	SPI.setBitOrder(MSBFIRST);
	SPI.endTransaction();

}
