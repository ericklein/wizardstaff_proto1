/*
  Project Name:   wizardstaff proto1
  Developer:      Eric Klein Jr. (temp2@ericklein.com)
  Description:    Testing accelerometer integration, first hardware build

  See README.md for target information, revision history, feature requests, etc.
*/

// Library initialization
#include <FastLED.h>
#include "LEDControl.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>

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
//LEDControl stripTwo(DATA_PIN_2,NUM_LEDS,ledsTwo); //for p2, 4 strips

// Instantiate accelerometer

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

  // initiate accelerometer
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
  sensors_event_t event; 
  accel.getEvent(&event);
  #ifdef DEBUG
    // Display the results (acceleration is measured in m/s^2)
    Serial.print("X: "); Serial.print(event.acceleration.x);
    Serial.print(" Y: "); Serial.print(event.acceleration.y);
    Serial.print(" Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2 ");
  #endif
 
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
  demo_loop(64,100);

  // ..or maybe counter-rotating chases
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
