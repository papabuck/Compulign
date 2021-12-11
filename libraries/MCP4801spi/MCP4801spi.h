/*
  MCP4801.h - Library for interacting with the Microchip MCP4801 
              8-Bit Voltage Output Digital-to-Analog Converter
              with Internal VREF and SPI Interface.
  Created by Craig Wm. Versek, 2012-05-13
  Released into the public domain.
 */

#include <Arduino.h>

#define GAIN_SHTDWN_BITS  0x1000

class MCP4801 {
public:
  	MCP4801(int csPin, int ldacPin );
 	int setOutput(int value);

private:
  int _csPin;
  int _ldacPin;
};
