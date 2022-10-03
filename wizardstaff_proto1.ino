/*
  Project Name:   wizardstaff proto1
  Description:    Testing accelerometer integration, first hardware build

  See README.md for target information, revision history, feature requests, etc.
*/

// configuration parameters
#include "config.h"

// Library initialization
#include <FastLED.h>
#include "LEDControl.h"
//#include <Wire.h> // only used by Adafruit accelerometer lib
//#include <Adafruit_Sensor.h>
//#include <Adafruit_ADXL343.h>
#include <SparkFun_ADXL345.h>

// Instantiate LED strips
CRGB ledsOne[LEDS_STRIP];
CRGB ledsTwo[LEDS_STRIP];
LEDControl stripOne(LEDS_STRIP,ledsOne); 
LEDControl stripTwo(LEDS_STRIP,ledsTwo);

// global variables
int staffMode, prevStaffMode;
int prevAccelX, prevAccelY, prevAccelZ; // last accelerometer measurements
uint32_t syncTime = 0;        // milliseconds tracker

extern void staffModeNormalUpdate();
extern void staffModeFlashlightUpdate();
extern void staffModePairUpdate();
extern void staffModeMusicUpdate();
extern void staffModeErrorUpdate();

// Instantiate accelerometer
// Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);
ADXL345 adxl = ADXL345();

void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial)
    debugMessage("wizardstaff p1 started");
  #endif

  initLEDStrips();
  initAccelerometer();

  // Visually indicate staff is ready for use
  staffMode = MODE_NORMAL;
}

void loop()
{
  // ? Do we carry accel and GPS locally or global?

  if ((millis() - syncTime) < GPS_UPDATE_INTERVAL) 
  {
    // update GPS position 
  }
  if ((millis() - syncTime) < ACCEL_UPDATE_INTERVAL) 
  {
    accelUpdate(); 
  }
  // update core clock counter
  syncTime = millis();

  // Handle battery level via MODE_ERROR

  // change or update current staff mode
  debugMessage(String("prevStaffMode ")+prevStaffMode);
  debugMessage(String("StaffMode ")+StaffMode);
  if (staffMode!=prevStaffMode)
  {
    switch(staffMode)
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
  prevStaffMode = staffMode;
  }
  //update LED strips
  stripOne.update();
  stripTwo.update();
  FastLED.show();
}

void accelUpdate()
{
  int accelX, accelY, accelZ; // Current accelerometer measurements

  // dump accelerometer values for comparison
  prevAccelX = accelX;
  prevAccelY = accelY;
  prevAccelZ = accelZ;
  
  // Accelerometer events to change MODE_XXX
  //   MODE_NORMAL - (default post boot, first cycle position via z-axis rapid in and out
  //   MODE_PAIR - drop quickly on y-axis
  //   MODE_MUSIC - second cycle position via via z-axis rapid in and out
  //   MODE_FLASHLIGHT - third cycle position via via z-axis rapid in and out
  //   MODE_ERROR - N/A (automatic)

  // Accelerometer events within MODE_XXX
  //   MODE_NORMAL - ?
  //   MODE_PAIR - ?
  //   MODE_MUSIC - ?
  //   MODE_FLASHLIGHT - none
  //   MODE_ERROR - none (power reset required to leave MODE)
    
  // Sparkfun accelerometer
  // Review high level accelerometer events
  byte interrupts = adxl.getInterruptSource();
 
  // Staff is double tapped
  // if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP))
  // // Right now, the only way to activate MODE_FLASHLIGHT is through MODE_NORMAL
  // {
  //   if(prevStaffMode = MODE_NORMAL)
  //     {
  //       debugMessage("Double tap switch from MODE_NORMAL to MODE_FLASHLIGHT");
  //       staffMode = MODE_FLASHLIGHT;
  //     }
  //   else if(prevStaffMode = MODE_FLASHLIGHT)
  //     {
  //       debugMessage("Double tap switch from MODE_FLASHLIGHT to MODE_NORMAL");
  //       staffMode = MODE_NORMAL;
  //     }
  // }
  
  // Activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY))
  {
      debugMessage("x axis activity detected");
  }
  adxl.readAccel(&accelX, &accelY, &accelZ);

  // Inactivity
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY))
  {
    debugMessage("no accelerometer activity");
    //jump to the next sensor to check?
  }
  // Display the results (acceleration is measured in m/s^2)
  debugMessage(String("X: "+accelX+" Y: "+accelY+" Z: "+accelZ+" m/s^2");

  //Adafruit accelerometer
  // sensors_event_t event; 
  // accel.getEvent(&event);
  // #ifdef DEBUG
  //   // Display the results (acceleration is measured in m/s^2)
  //   Serial.print("X: "); Serial.print(event.acceleration.x);
  //   Serial.print(" Y: "); Serial.print(event.acceleration.y);
  //   Serial.print(" Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2 ");
  // #endif
}

void initLEDStrips()
{
  FastLED.addLeds<WS2812B, ledStrip1DataPin, GRB>(ledsOne,LEDS_STRIP);
  FastLED.addLeds<WS2812B, ledStrip2DataPin, GRB>(ledsTwo,LEDS_STRIP);
  debugMessage("LED strips initialized");
}

void debugMessage(String messageText)
{
  #ifdef DEBUG
    Serial.println(messageText);
  #endif
}

void initAccelerometer()
{
    // initiate accelerometer
  /* Adafruit accelerometer initialization
  if(!accel.begin())
  {
    // There was a problem detecting the ADXL345
    debugMessage("ADXL345 not detected, check wiring");
    staffMode = MODE_ERROR;
  }

  // Higher values = wider measurement range. Lower values = more sensitivity
  // accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);

  */

  // Sparkfun accelerometer initialization
  adxl.powerOn();                     // Power on the ADXL345
  adxl.setRangeSetting(8);           // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher values = wider measurement range. Lower values = more sensitivity
 
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
  
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in the axes (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(5);         // How many seconds of no activity is inactive?

  /*adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 */

 /* 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 */

  // Setting all interupts to take place on INT1 pin
  adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  //adxl.FreeFallINT(1);
  //adxl.doubleTapINT(1);
  //adxl.singleTapINT(1);
  debugMessage("accelerometer initalized");
}