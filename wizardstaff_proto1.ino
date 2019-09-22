/*
  Project Name:   wizardstaff proto1
  Developer:      Eric Klein Jr. (temp2@ericklein.com)
  Description:    Testing accelerometer integration, first hardware build

  See README.md for target information, revision history, feature requests, etc.
*/

// Library initialization
#include <FastLED.h>
#include "LEDControl.h"
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_ADXL343.h>
#include <SparkFun_ADXL345.h>

// Conditional code definitions
#define stripLPD8806
//#define stripWS2812B
#define DEBUG

// Assign LED specific information
#ifdef stripWS2812B
  #define ledStrip0DataPin  5
#endif

// conditional can be removed for p2
#ifdef stripLPD8806
  #define ledStrip0DataPin  11
  #define ledStrip0ClockPin 12
#endif

#define NUM_LEDS  18 // NUM_LEDS consistent across strips for wizardstaff

// Wizardstaff modes
#define MODE_BOOT       0 // setup(), which exits to MODE_NORMAL or MODE_ERROR
#define MODE_PAIR       1 // find the matching wizardstaff
#define MODE_MUSIC      2 // reacts to microphone audio
#define MODE_FLASHLIGHT 3 // lots of light
#define MODE_ERROR      4 // alert user to error condition
#define MODE_NORMAL     5 // rotating animations

// global variables
int StaffMode;
int StaffPrevMode;
CRGB ledsOne[NUM_LEDS];
//CRGB ledsTwo[NUM_LEDS]; // for p2, 4 strips

// Instantiate led strips
LEDControl stripOne(ledStrip0DataPin, NUM_LEDS,ledsOne); // Pin isn't used in library, checking with David on this 
//LEDControl stripTwo(DATA_PIN_2,NUM_LEDS,ledsTwo); //for p2, 4 strips

// Instantiate accelerometer via i2c
// Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);
ADXL345 adxl = ADXL345();


void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) ;
  #endif

  StaffMode = MODE_BOOT;
  StaffPrevMode = MODE_BOOT;

  // Initialize our LED strips
  #ifdef stripWS2812B
    FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(ledsOne,NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN_2, GRB>(ledsTwo,NUM_LEDS);
  #endif

  #ifdef stripLPD8806
    FastLED.addLeds<LPD8806, ledStrip0DataPin, ledStrip0ClockPin, GRB>(ledsOne, NUM_LEDS);
  #endif

  // Indicate start of boot sequence via green therometer (50%)

  #ifdef DEBUG
   Serial.println("LED strip(s) initalized");
  #endif

  /* // initiate accelerometer
  if(!accel.begin())
  {
    // There was a problem detecting the ADXL345
    #ifdef DEBUG
      Serial.println("ADXL345 not detected, check wiring");
    #endif
    StaffMode = MODE_ERROR;
  }

  // Higher values = wider measurement range. Lower values = more sensitivity
  // accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);

  */

  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity
 
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 
  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);


  // Indicate end of boot sequence via green therometer (100%)
  #ifdef DEBUG
    Serial.println("accelerometer initalized");
  #endif

  StaffMode = MODE_NORMAL;
}

void loop()
{
  // in future versions, read battery level

  //read the accelerometer
  /* sensors_event_t event; 
  accel.getEvent(&event);
  #ifdef DEBUG
    // Display the results (acceleration is measured in m/s^2)
    Serial.print("X: "); Serial.print(event.acceleration.x);
    Serial.print(" Y: "); Serial.print(event.acceleration.y);
    Serial.print(" Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2 ");
  #endif
  */
 
 // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  // Output Results to Serial
  /* UNCOMMENT TO VIEW X Y Z ACCELEROMETER VALUES */  
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z); 
  
  ADXL_ISR();

  // compare accel values to previous values, determine if mode switch is required
  
  /* Accelerometer events to change between modes

    Double taps?
    up, down, then tap(s)?
    shake?

    MODE_BOOT - N/A (automatic)
    MODE_PAIR
    MODE_MUSIC
    MODE_FLASHLIGHT
    MODE_ERROR - N/A (automatic)
    MODE_NORMAL
  */

  /* Intra-mode accelerometer events
    MODE_BOOT - N/A
    MODE_PAIR - N/A
    MODE_MUSIC - N/A
    MODE_FLASHLIGHT - N/A
    MODE_ERROR - N/A
    MODE_NORMAL - shake, x axis for flow
  */

  // Events to be detected
    // detect vertical movement and flow LEDS
    // long hold upside down
    // spinning
    // shaking
    //      horizontal
    //      vertical
    // horizontal
    // single tap on ground
    // double tap on ground

  // change or handle current mode

  switch(StaffMode)
  {
    case MODE_NORMAL:
      staffModeNormalUpdate();
      break;
    case MODE_FLASHLIGHT:
      staffModeFlashlightUpdate();
      break;
    case MODE_MUSIC:
      staffModeMusicUpdate();
      break;
    case MODE_PAIR:
      staffModePairUpdate();
      break;
    case MODE_ERROR:
      staffModeErrorUpdate();
      break;
  }
}

void staffModeNormalUpdate()
{
  #ifdef DEBUG
    Serial.println("handling MODE_NORMAL");
  #endif
  // what mode where we in before? handle transitions if needed
  // cycle through some animations for proto1
  stripOne.setRunFwd(CRGB::Blue);
  //stripTwo.setRunFwd(CRGB::Purple);
  demo_loop(32,100);

 /*  // ..or maybe counter-rotating chases
  stripOne.setRunFwd(CRGB::Blue);
  //stripTwo.setRunRev(CRGB::Red);
  demo_loop(64,60);

  // ...or rainbow chase
  stripOne.setRainbowFwd();
  //stripTwo.setRainbowRev();
  demo_loop(64,150);
  
  // ..or Cylon eyes
  stripOne.setCylon(CRGB::Red);
  //stripTwo.setCylon(CRGB::Red);
  demo_loop(64,100);

*/
}

void staffModeFlashlightUpdate()
{
  #ifdef DEBUG
    Serial.println("handling MODE_FLASHLIGHT");
  #endif
  // mode doesn't care about previous LED status
  // solid white with high brightness
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::White);
  stripOne.update();
  FastLED.show();
}

void staffModePairUpdate()
{
  #ifdef DEBUG
    Serial.println("handling MODE_PAIR");
    Serial.println("not handled in proto1");
  #endif
}

void staffModeMusicUpdate()
{
  #ifdef DEBUG
    Serial.println("handling MODE_MUSIC");
    Serial.println("not handled in proto1");
  #endif
}

void staffModeErrorUpdate()
{
  #ifdef DEBUG
    Serial.println("handling MODE_ERROR");
  #endif
  // mode doesn't care about previous LED status
  // blink red for proto1
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Red);
  stripOne.update();
  FastLED.show();
}

void demo_loop(int count, int delayms){
  for(int i=0;i<count;i++) {
    stripOne.update();
    //stripTwo.update();
    FastLED.show();
    // We run all strips at a 100ms clock speed (10 Hz)
    delay(delayms);
  }
}

/********************* ISR *********************/
/* Look for Interrupts and Triggered Action    */
void ADXL_ISR() {
  
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource();
  
  // Free Fall Detection
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
    Serial.println("*** FREE FALL ***");
    //add code here to do when free fall is sensed
  } 
  
  // Inactivity
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
    Serial.println("*** INACTIVITY ***");
     //add code here to do when inactivity is sensed
  }
  
  // Activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    Serial.println("*** ACTIVITY ***"); 
     //add code here to do when activity is sensed
  }

    // Tap Detection
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
    Serial.println("*** TAP ***");
     //add code here to do when a tap is sensed
    StaffMode = MODE_FLASHLIGHT;
    delay(500);
  } 
  
  // Double Tap Detection
  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
    Serial.println("*** DOUBLE TAP ***");
     //add code here to do when a 2X tap is sensed
    StaffMode = MODE_NORMAL;
  }
}
