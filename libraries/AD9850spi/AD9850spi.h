/**************************************************************************/
/*! 
      @file     AD9850.h
    @author   Louis J Haskell
	@license  BSD 
	
	Driver for AD9850 DDS frequency generator board


	@section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

 #include "Arduino.h"


class AD9850{
 public:
  AD9850( int in_fqud, int in_reset );
  void setFrequency( int32_t frequency );
  void setSerialMode();

 private:

   uint8_t  fqud, reset;
};
