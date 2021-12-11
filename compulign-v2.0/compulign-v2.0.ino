/*
 * DDS based Function and Sweep Generator
 * Lou Haskell Copyright 2017 . MIT license
 * Version V2.0
 */


// Set up optional debugging flag
// #define DEBUG 1

int tmp;


class VoltageRef {       // class for the 5 volt reference reference using the PWM pin
  public:
              void setVoltage(int voltageValue) {
                  analogWrite(REFVOLTAGEPIN, TENTHVOLT * voltageValue);
            }
            
  private:
            const int REFVOLTAGEPIN = 10;
            const int TENTHVOLT = 5;
};

class VoltageInput {
    public:
            const int  MAX_ADC_VAL = 1024;
   
            int readInputVoltage() {
              return(analogRead(ADC_PIN));
            }
    private:
            const int ADC_PIN = A0;
};

#include <SPI.h>
#include <UTFT.h>
extern uint8_t Inconsola[];
extern uint8_t BigFont[];

class View {

  private:
          UTFT lcd;

          const int LCD_RESET = 4;
          const int LCD_CS =    5;
          const int LCD_DC =    6;

          const int BLACK =   0; 
          const int BLUE  =   0x001F;
          const int RED   =   0xF800;
          const int GREEN =   0x07E0;
          const int CYAN  =   0x07FF;
          const int MAGENTA = 0xF81F;
          const int YELLOW =  0xFFE0;
          const int WHITE =   0xFFFF;
          
          const int HILITE_COLOR = WHITE;
          const int FUNC_COLOR =   BLUE;
          const int SWEEP_COLOR =  RED;
          const int BANDWIDTH_COLOR = GREEN;
          const int MODE_COLOR =   YELLOW;

          #define DISPLAY_WIDTH          320
          const int DISPLAY_HEIGHT  =    240;
          const int DISPLAY_ITEMS  =     4;
          const int DISPLAY_DIGITS  =    8;
          const int DIGIT_PRINT_WIDTH =  19;  // pixels of special print width for freq digits
          const int VALS_PRINT_POS  =    155;
          const int VALS_LABEL_POS  =    125;
          const int OPT_PRINT_POS  =     70;
          const int OPT_PRINT_POSPT   =  90;
          const int OPT_PRINT_POS2  =    94;
          const int MODE_Y_POS   =       5;
          const int RATE_Y_POS   =       25;
          const int MARKER_Y_POS   =     45;
          const int BIAS_Y_POS   =       65;
          const int GRAPH_START_X   =    0;
          const int GRAPH_START_Y  =     109;
          const int GRAPH_WIDTH   =      320;
          const int GRAPH_HEIGHT   =     128;

          int graphVals[DISPLAY_WIDTH];
          char lastMarkerVal;

         displayFixedItems() {
            lcd.setFont(BigFont); 
            lcd.setBackColor(BLACK);
            lcd.setColor(FUNC_COLOR);
            lcd.print("FN", VALS_LABEL_POS, 5);
            lcd.setColor(SWEEP_COLOR); 
            lcd.print("RF", VALS_LABEL_POS, 37); 
            lcd.setColor(BANDWIDTH_COLOR);  
            lcd.print("BW", VALS_LABEL_POS, 69);
            lcd.setColor(CYAN);  
            lcd.print("Mode", 0, MODE_Y_POS);
            lcd.print("Rate", 0, RATE_Y_POS);
            lcd.print("Mark", 0, MARKER_Y_POS);
            lcd.print("Bias", 0, BIAS_Y_POS);
            lcd.fillCircle( OPT_PRINT_POSPT, BIAS_Y_POS + 12, 2);
          }

          printFreqNum(int32_t num, int numcolor, int printPos, char digSelected) {
            char d;
            lcd.setFont(Inconsola);
            lcd.setColor(numcolor);
            for(d = 0; d < DISPLAY_DIGITS; d++) {
                if( d == digSelected) {
                  lcd.setColor(HILITE_COLOR);
                  lcd.printChar(num % 10 + '0', ((7 - d) * DIGIT_PRINT_WIDTH) + VALS_PRINT_POS, printPos);
                  lcd.setColor(numcolor);
                } else {
                  lcd.printChar(num % 10 + '0', ((7 - d) * DIGIT_PRINT_WIDTH) + VALS_PRINT_POS, printPos);
                }
                
                num = num / 10;
            }
          }

  public:
 
 
          init() {
            int i;
            
            lcd.InitLCD(LANDSCAPE);
           // lcd.clrScr();

            lcd.setColor(BLACK);
            lcd.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

            //display splash screen
            lcd.setFont(BigFont);
            
            lcd.printC("CompuLign" , CENTER, 0, WHITE);
            lcd.printC("L. J. Haskell", CENTER, 40, BLUE);
            lcd.printC("Copyright 2017", CENTER, 60, GREEN);
            lcd.printC("Version 2.0", CENTER, 80, RED);
            delay(2000);
            lcd.setColor(BLACK);
            lcd.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);            
              
            for(i = 0; i< DISPLAY_WIDTH; i++) {
              graphVals[i] = 0;
            }
              
            displayFixedItems();

            
          }
          
          displayFuncFreq(int32_t fnFreq, char digSelected){
            printFreqNum( fnFreq, FUNC_COLOR, 0, digSelected);
          }
          
          displaySweepFreq(int32_t swpFreq, char digSelected){
            printFreqNum( swpFreq, SWEEP_COLOR, 32, digSelected);
          }
          
          displayBWFreq(int32_t bwFreq, char digSelected){
            printFreqNum( bwFreq, BANDWIDTH_COLOR, 64,  digSelected);
          }
          
          displayMode(char *mode, char digSelected) {
              lcd.setFont(BigFont);
              if(digSelected <0)
                  lcd.setColor(MODE_COLOR);
              else
                  lcd.setColor(HILITE_COLOR);
              lcd.print( mode, OPT_PRINT_POS, MODE_Y_POS);
          }
          
          displayRate(char rate, char digSelected) {
              lcd.setFont(BigFont);
              if(digSelected <0)
                  lcd.setColor(MODE_COLOR);
              else
                  lcd.setColor(HILITE_COLOR);
              lcd.printChar( rate + '0', OPT_PRINT_POS, RATE_Y_POS);
          }
          
          displayMarker(char marker, char digSelected){
              lcd.setFont(BigFont);
              if(digSelected <0)
                  lcd.setColor(MODE_COLOR);
              else
                  lcd.setColor(HILITE_COLOR);
              lcd.printChar( marker +'0', OPT_PRINT_POS, MARKER_Y_POS); 
          }
          
          displayBias(char bias, char digSelected){
              lcd.setFont(BigFont);
              if(digSelected != 1)
                  lcd.setColor(MODE_COLOR);
              else
                  lcd.setColor(HILITE_COLOR);
              lcd.printChar( bias/10 + '0', OPT_PRINT_POS, BIAS_Y_POS);
              if(digSelected != 0)
                  lcd.setColor(MODE_COLOR);
              else
                  lcd.setColor(HILITE_COLOR);
              lcd.printChar( bias%10 + '0', OPT_PRINT_POS2, BIAS_Y_POS);
          }

 
          
          drawMarkers(int marker, int color) {
              int markerPos;
              
               if( marker > 0 ) {
                  markerPos = GRAPH_WIDTH/20 * marker;
                  
                  lcd.setColor(color);
                  lcd.drawLine(GRAPH_WIDTH/2 - markerPos, GRAPH_START_Y,GRAPH_WIDTH/2 - markerPos, GRAPH_START_Y+GRAPH_HEIGHT);
                  lcd.drawLine(GRAPH_WIDTH/2 + markerPos, GRAPH_START_Y,GRAPH_WIDTH/2 + markerPos, GRAPH_START_Y+GRAPH_HEIGHT);
             
             }
          }
          
          startGraph(int markerVal) {

              lcd.setColor(WHITE);
              //draw line marker at center of sweep, i.e. reference frequency 
              lcd.drawLine(GRAPH_WIDTH/2,GRAPH_START_Y,GRAPH_WIDTH/2, GRAPH_START_Y+GRAPH_HEIGHT);  

              if( lastMarkerVal != markerVal) {
                  drawMarkers(lastMarkerVal, BLACK);
      
              }
              lastMarkerVal = markerVal;
              
              drawMarkers(markerVal, GREEN);
            
              lcd.setColor(WHITE);
          }

          drawGraphPixel(int x, int y, int maxval) {
               int yVal;

               lcd.setColor(WHITE);
               yVal = GRAPH_START_Y + y / (maxval / GRAPH_HEIGHT);
               graphVals[x] = yVal;
               lcd.drawPixel(x, yVal);
                          
          }
          
          clearGraph() {
              int i;

              lcd.setColor(BLACK);
              for(i = 0; i < DISPLAY_WIDTH; i++)
                lcd.drawPixel(i, graphVals[i]);
              
         }
         
          clearGraphArea() {
              lcd.setColor(BLACK);
              lcd.fillRect(GRAPH_START_X, GRAPH_START_Y, GRAPH_START_X + GRAPH_WIDTH, GRAPH_START_Y + GRAPH_HEIGHT);
          }

};

#include <rotary2.h>
#include <AD9850spi.h>
#include <MCP4801spi.h>

class Controller {
   private:

          View view;
          VoltageRef voltageRef;
          VoltageInput voltsIn;
          
          // AD9850 DDS generators - Sweep and Function
          const int SWP_FQ_UD =  A1;       // connect to freq update pin (FQ)
          const int SWP_RESET =  8;      // connect to reset pin (RST).
          const int FUNC_FQ_UD = A4;       // connect to freq update pin (FQ)
          const int FUNC_RESET = 9;      // connect to reset pin (RST).
  
          //MCP4801 DAC
          const int DAC_CS =   A3;
          const int DAC_LDAC = A2;

          
          
          
           AD9850 sweepgen = AD9850( SWP_FQ_UD,  SWP_RESET );
           AD9850 funcgen = AD9850(  FUNC_FQ_UD,   FUNC_RESET);
           MCP4801 dac = MCP4801(  DAC_CS,  DAC_LDAC);
          
          int32_t funcFreq = 400; 
          int32_t sweepFreq = 455000; 
          int32_t bandwidthFreq = 20000;
          
          char funcMode = 0;   
          const int  MODE_FIX  = 0;
          const int  MODE_EXT  = 1;
          const int  MODE_INT  = 2;
          const int  MODE_MAX = 2;
          const char *modeDisplay[3] = {"FIX", "SWe", "SWi"};
          char sweepRate = 9;   
          char markerVal = 0;
          char lastMarkerVal = 0;
          char biasVal = 0;

          const char ROTARY_DT =  2;    // must be interrupt line!
          const char ROTARY_CLK = 3;    // must be interrupt line!
          const char ROTARY_SW =  7;
          const char MAXPOS = 28;  // 3 * 8 for freq digits plus 5 digits for mode functions minus 1 since we count 0
          
          char selectorPos = 0;
          char prevSelectorPos = 0;
          bool changeFlag = 0;
          char posChange = 0;
          char digitChange = 0;

          // all constants for the selector operation
          #define SELMODE   0
          #define SELRATE   0x10
          #define SELMARK   0x20
          #define SELBIAS   0x30
          #define SELFN     0x40
          #define SELRF     0x50
          #define SELBW     0x60

          const char SELECT_DECODE[29] = {  SELMODE, SELRATE, SELMARK, SELBIAS | 1, SELBIAS,
                            SELFN, SELFN | 1,SELFN | 2,SELFN | 3 ,SELFN | 4,SELFN | 5,SELFN | 6,SELFN | 7,
                            SELRF, SELRF | 1,SELRF | 2,SELRF | 3 ,SELRF | 4,SELRF | 5,SELRF | 6,SELRF | 7,
                            SELBW, SELBW | 1,SELBW | 2,SELBW | 3 ,SELBW | 4,SELBW | 5,SELBW | 6,SELBW | 7
                                          
          };
          const int32_t digitPowers[8] =  {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};

 
          Rotary rotary = Rotary( ROTARY_DT,   ROTARY_CLK,  ROTARY_SW);


          bool selectorDigitChanged() {
            if(posChange != 0 || digitChange != 0)
                return true;
              else
                return false;
          }
          
          processSelectorChange() {
            char digit, field, code, val, oldMode, prevDigit, prevCode, prevField;
   
            code = SELECT_DECODE[selectorPos];
            digit = code & 0xF;
            field = code & 0xF0;
            
            if( posChange != 0 ) {
                // set the old position back to normal and new position to hilite
                // if old and new are in same field, no need to make separate call to turn off old digit
                prevCode = SELECT_DECODE[prevSelectorPos];
                prevDigit = prevCode & 0xF;
                prevField = prevCode & 0xF0;

                switch(prevField) {
                  case SELMODE: view.displayMode(modeDisplay[funcMode],-1);
                                break;

                  case SELRATE: view.displayRate(sweepRate,-1);
                                break;

                  case SELMARK: view.displayMarker(markerVal,-1);
                                break;                            

                  case SELBIAS: view.displayBias(biasVal, -1);
                                break;
                                
                  case SELFN: view.displayFuncFreq(funcFreq, -1);
                                break; 
                                                              
                  case SELRF: view.displaySweepFreq(sweepFreq, -1);
                                break;

                  case SELBW: view.displayBWFreq(bandwidthFreq, -1);
                                break;                  
                                  
                }

                switch(field) {
                  case SELMODE: view.displayMode(modeDisplay[funcMode],digit);
                                break;

                  case SELRATE: view.displayRate(sweepRate,digit);
                                break;

                  case SELMARK: view.displayMarker(markerVal,digit);
                                break;                            

                  case SELBIAS: view.displayBias(biasVal, digit);
                                break;
                                
                  case SELFN: view.displayFuncFreq(funcFreq, digit);
                                break; 
                                                              
                  case SELRF: view.displaySweepFreq(sweepFreq, digit);
                                break;

                  case SELBW: view.displayBWFreq(bandwidthFreq, digit);
                                break;                  
                                  
                }
              posChange = 0;
            }

            if( digitChange != 0) {

                switch(field) {

                  case SELMODE:   oldMode = funcMode;
                                  funcMode += digitChange;
                                  funcMode = rangeCheck(funcMode, MODE_MAX);
                                  view.displayMode(modeDisplay[funcMode],0);
                                  if(funcMode != MODE_INT && oldMode == MODE_INT) {
                                    view.clearGraphArea();
                                  }
                                  break;
                                  
                  case SELRATE:   sweepRate += digitChange;                      
                                  sweepRate = rangeCheck(sweepRate,9);
                                  view.displayRate(sweepRate,0);
                                  break;
                                  
                  case SELMARK:   markerVal += digitChange;
                                  markerVal = rangeCheck(markerVal,9);
                                  view.displayMarker(markerVal,0);
                                  break;
                                  
                  case SELBIAS:   if( digit == 0) {
                                      val = biasVal % 10;
                                      val += digitChange;
                                      val = rangeCheck(val,9);
                                      biasVal = (biasVal /10)*10 + val;
                                  } else {                        
                                      val = biasVal / 10;
                                      val += digitChange;
                                      val = rangeCheck(val,4);
                                      biasVal = (val * 10) + biasVal % 10;                                  
                                   }
                                  
                                  voltageRef.setVoltage(biasVal);
                                  view.displayBias(biasVal, digit);
                                  tmp = biasVal;Serial.println(tmp);
                                  break;
                                  
                  case SELFN:     val = (funcFreq / digitPowers[digit]) % 10;
                                  val += digitChange;
                                  val = rangeCheck(val, 9);
                                  funcFreq = calcDigitChange(funcFreq, val, digit);
                                  funcgen.setFrequency(funcFreq);
                                  view.displayFuncFreq(funcFreq, digit);
                                  break;
                                  
                  case SELRF:     val = (sweepFreq / digitPowers[digit]) % 10;
                                  val += digitChange;
                                  val = rangeCheck(val, 9);
                                  sweepFreq = calcDigitChange(sweepFreq, val, digit);
                                  sweepgen.setFrequency(sweepFreq);
                                  view.displaySweepFreq(sweepFreq, digit);
                                  break;
                                  
                  case SELBW:     val = (bandwidthFreq / digitPowers[digit]) % 10;
                                  val += digitChange;
                                  val = rangeCheck(val, 9);
                                  bandwidthFreq = calcDigitChange(bandwidthFreq, val, digit);
                                  view.displayBWFreq(bandwidthFreq, digit);
                                  break;
                }

              digitChange = 0;
            }
 
          }
          
          char rangeCheck(char in, char maxval) {
                   if (in < 0)
                        return 0;
                   else if(in > maxval)
                        return maxval; 
                   return in;
          }

          uint32_t calcDigitChange(uint32_t in, char val, char digit) {
                uint32_t tmp;
                char oldDigit;
                
                tmp = in / digitPowers[digit];
                oldDigit = tmp % 10;
                in = in - (oldDigit * digitPowers[digit]) + (val * digitPowers[digit]);
                return(in);
          }
          
         // Sweep routine for the external sweep generator plus ramp sync output 
          SweepExternal() {        

              uint16_t i, sweep_inc, sweep_lvl;
              uint32_t f, freq_inc;
              const int EXTSWEEPSTEPS = 128;
              const int DACMAX = 256;
              const int SWEEP_DELAY_STEP = 300;
              
              freq_inc = bandwidthFreq / EXTSWEEPSTEPS;
              sweep_inc = DACMAX / EXTSWEEPSTEPS;
              f = sweepFreq - bandwidthFreq/2;

              sweep_lvl = 0;
              // need to update sweepRate and markerVal beofre each loop in case they changed. Add code here
              for (i = 0; i < EXTSWEEPSTEPS; i++) {

                     // if the display is being updated, bail out to allow changes and clean redraw
                    if(selectorDigitChanged() )
                       return;
    
                    dac.setOutput(sweep_lvl);
                    sweepgen.setFrequency(f);

                    // delay between steps according to sweep delay setting
                    delayMicroseconds((9 -sweepRate) * SWEEP_DELAY_STEP);
                    //if the middle of the sweep, i.e. target frequency, delay so there is a bright spot on screen
                    if( i == EXTSWEEPSTEPS/2) { 
                        delayMicroseconds(((9 -sweepRate) + markerVal)*SWEEP_DELAY_STEP );
                    }
    
                    f = f + freq_inc;
                    sweep_lvl = sweep_lvl + sweep_inc;
             }
 
        }
          
          SweepInternal() {
            
               uint16_t i, sweep_inc, val;
               uint32_t f, freq_inc;
               const int INTSWEEPSTEPS =  320;
              const int SWEEP_DELAY_STEP = 300;
              
              freq_inc = bandwidthFreq / INTSWEEPSTEPS;
              sweep_inc = INTSWEEPSTEPS;
              f = sweepFreq - bandwidthFreq/2;
            
              // clear display area by writing over the previous trace in background color
              
              view.clearGraph();
              
              view.startGraph(markerVal);
              
              for (i = 0; i < INTSWEEPSTEPS; i++) {
            
                  // if the display is being updated, bail out to allow changes and clean redraw
                  if(selectorDigitChanged() )
                    return;
                  
                  sweepgen.setFrequency(f);
                
                  view.drawGraphPixel(i, voltsIn.readInputVoltage(), voltsIn.MAX_ADC_VAL);
                  // delay between steps according to sweep delay setting
                  delayMicroseconds((9 -sweepRate) * SWEEP_DELAY_STEP);
                  
                 f = f + freq_inc;
              }
                
            }

 
  
  public:

          void operate() {

                   delay(1);  // this is a hack to make selector work. Not sure why yet
                   processSelectorChange();
  
                   if(funcMode == MODE_EXT) 
                      SweepExternal(); 

                    if(funcMode == MODE_INT) 
                      SweepInternal();         
          }
  
 
          init() {
              SPI.begin();
              view.init();

              funcgen.setFrequency(funcFreq);
              view.displayFuncFreq(funcFreq, -1);

              sweepgen.setFrequency(sweepFreq);
              view.displaySweepFreq(sweepFreq, -1);
              
              view.displayBWFreq(bandwidthFreq, -1);
              view.displayMode(modeDisplay[funcMode], 0);
              view.displayRate(sweepRate, -1);
              view.displayMarker(markerVal, -1);

              voltageRef.setVoltage(biasVal);
              view.displayBias(biasVal, -1);
           
          }

          void rotaryInterruptHandler(){
            char switch_state;
            unsigned char result = rotary.process();
   
            // if processing a change at base level, then ignore this change   
            if(posChange != 0 || digitChange != 0) {

               return;
            }
            
            switch_state = result & 0x80;
            result &= 0x7F;   
                        
            if(switch_state == LOW) {  // switch is pressed we process digit changes
                 
                if (result == DIR_CW) {
                    digitChange = 1;
                } else if (result == DIR_CCW) {
                    digitChange = -1;
                              
                }  
                
              
            } else {  // if switch is not pressed we process digit position change

                
                if (result == DIR_CW) {
                     prevSelectorPos = selectorPos; 
                    selectorPos++;
                    if(selectorPos >  MAXPOS)
                      selectorPos = 0;
                    posChange = 1;
                } else if (result == DIR_CCW) {
                    prevSelectorPos = selectorPos; 
                    selectorPos--;
                    if(selectorPos < 0)
                      selectorPos =  MAXPOS;
                    posChange = 1;                             
                } 
    
 
            }

         }

        
          
 
};


Controller controller ;

// for now it looks like these must be globals but will look at sometime to get into View class


// process used simply as a static function to make the Interrupt Service Routine work. Just calls the interrupt handler
 void rotaryInterrupt() {

    controller.rotaryInterruptHandler();

 }
 
void setup() {

  //      if(DEBUG)
 //        Serial.begin(19200);
 //     Serial.println("START");
          
        controller.init();
        
         // when everything else is initialized, turn on interrupts to allow for changes via the selector
            attachInterrupt(0, rotaryInterrupt, CHANGE);
            attachInterrupt(1, rotaryInterrupt, CHANGE);


 }

 void loop() {

    
    controller.operate();
  
 }

 
