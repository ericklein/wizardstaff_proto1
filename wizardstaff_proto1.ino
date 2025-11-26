/*
  Project:        wizardstaff proto1
  Description:    Testing accelerometer integration, first hardware build

  See README.md for target information
*/

#include "config.h"               // hardware and internet configuration parameters
#include <FastLED.h>              // https://github.com/FastLED/FastLED, LED control
#include <LEDControl.h>           // multi LED strip async control
// #include <Adafruit_Sensor.h>
// #include <Adafruit_ADXL343.h>
// #include <SparkFun_ADXL345.h>

// Instantiate LED strips
CRGB ledsOne[ledStripPixelCount];
CRGB ledsTwo[ledStripPixelCount];
LEDControl stripOne(ledStripPixelCount,ledsOne); 
LEDControl stripTwo(ledStripPixelCount,ledsTwo);

// Instantiate accelerometer
// Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);
// ADXL345 accel = ADXL345();

// global variables
uint32_t timeLastAccelSyncMS = 0;
uint32_t timeLastGPSSyncMS = 0;
enum staffModes staffCurrentMode = mNormal;
uint8_t staffPreviousMode = mNormal;
// int prevAccelX, prevAccelY, prevAccelZ; // last accelerometer measurements

void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial)
    debugMessage("wizardstaff p1 started",1);
  #endif

  // let user know staff is powering up
  initLEDStrips();
  stripOne.setOneColor(CRGB::Yellow);
  stripTwo.setOneColor(CRGB::Yellow);
  FastLED.show();

  // initialize accelorometer
  if (!accelInit())
  {
    debugMessage("Accelerometer failed to initialize",1);
    staffCurrentMode = mError;
    // leave the LEDs in this mode for a bit (or forever)
    // or reboot the hardware
  }

  // initialize GPS
  if (!gpsInit())
  {
    debugMessage("GPS failed to initialize",1);
    staffCurrentMode = mError;
    // leave the LEDs in this mode for a bit (or forever)
    // or reboot the hardware
  }
}

void loop()
{
  // has the staff moved in a way to trigger a change?
  if ((millis() - timeLastAccelSyncMS) > accelSampleIntervalMS) 
  {
    accelUpdate();
    timeLastAccelSyncMS = millis();
  }

  // is it time to update the staff's GPS position?
  if ((millis() - timeLastGPSSyncMS) > gpsSampleIntervalMS) 
  {
    gpsUpdate();
    timeLastGPSSyncMS = millis();
  }

  // change or update current staff mode
  if (staffCurrentMode!=staffPreviousMode)
  {
    debugMessage(String("Staff mode was ") + staffPreviousMode + ", now " + staffCurrentMode,1);
    switch(staffCurrentMode)
    {
      case mNormal:
        staffUpdateNormal();
        break;
      case mFlashlight:
        staffUpdateFlashlight();
        break;
      case mMusic:
        staffUpdateMusic();
        break;
      case mPair:
        staffUpdatePair();
        break;
      case mError:
        staffUpdateError();
        break;
    }
    staffPreviousMode = staffCurrentMode;
  }

  //update LED strips
  stripOne.update();
  stripTwo.update();
  FastLED.show();
  delay(100);          // 10Hz clock for driving animations
}

void initLEDStrips()
{
  FastLED.addLeds<WS2812B, ledStrip1DataPin, GRB>(ledsOne,ledStripPixelCount);
  FastLED.addLeds<WS2812B, ledStrip2DataPin, GRB>(ledsTwo,ledStripPixelCount);
  debugMessage("LED strips initialized",1);
}

bool gpsInit()
// Description: Initializes the GPS
// Parameters: NA
// Output: true if initialization successful
// Improvement: ?
{
  #ifdef HARDWARE_SIMULATE
    return true;
  #endif
  debugMessage("GPS module initialized",1);
  return true;
 }

void gpsUpdate()
{
  debugMessage("GPS location updated",1);
  // test only
  staffCurrentMode = mFlashlight;
}

bool accelInit()
// Description: Initializes the ADXL 343/345 accelerometer
// Parameters: NA
// Output: true if initialization successful
// Improvement: ?
{
  #ifdef HARDWARE_SIMULATE
    return true;
  #endif

  debugMessage("Accelerometer initialized",1);
  return true;

  //   Adafruit accelerometer initialization
  //   if(!accel.begin())
  //   {
  //     // There was a problem detecting the ADXL345
  //     #ifdef DEBUG
  //       Serial.println("ADXL345 not detected, check wiring");
  //     #endif
  //     staffCurrentMode = MODE_ERROR;
  //     debugMessage("ADXL345 not detected, check wiring");
  //     staffCurrentMode = MODE_ERROR;
  //   }

  //   // Higher values = wider measurement range. Lower values = more sensitivity
  //   // accel.setRange(ADXL345_RANGE_16_G);
  //   // accel.setRange(ADXL345_RANGE_8_G);
  //   // accel.setRange(ADXL345_RANGE_4_G);
  //   // accel.setRange(ADXL345_RANGE_2_G);

  //   // Sparkfun accelerometer initialization
  //   accel.powerOn();                     // Power on the ADXL345

  //   accel.setRangeSetting(16);           // Give the range settings
  //                                       // Accepted values are 2g, 4g, 8g or 16g
  //                                       // Higher Values = Wider Measurement Range
  //                                       // Lower Values = Greater Sensitivity
  //   accel.setRangeSetting(8);           // Accepted values are 2g, 4g, 8g or 16g
  //                                       // Higher values = wider measurement range. Lower values = more sensitivity
   
  //   accel.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  //   accel.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes (1 == ON, 0 == OFF)
  //   accel.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
   
  //   accel.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    
  //   accel.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in the axes (1 == ON, 0 == OFF)
  //   accel.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  //   accel.setTimeInactivity(10);         // How many seconds of no activity is inactive?
  //   accel.setTimeInactivity(5);         // How many seconds of no activity is inactive?

  //   accel.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
  //   accel.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
   
  //   // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  //   accel.setTapThreshold(50);           // 62.5 mg per increment
  //   accel.setTapDuration(15);            // 625 μs per increment
  //   accel.setDoubleTapLatency(80);       // 1.25 ms per increment
  //   accel.setDoubleTapWindow(200);       // 1.25 ms per increment
   
  //   // Set values for what is considered FREE FALL (0-255)
  //   accel.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  //   accel.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
   
  //  */

  //   // Setting all interupts to take place on INT1 pin
  //   //accel.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
  //   accel.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
  //                                                         // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
  //                                                         // This library may have a problem using INT2 pin. Default to INT1 pin.
    
  //   // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  //   accel.InactivityINT(1);
  //   accel.ActivityINT(1);
  //   accel.FreeFallINT(1);
  //   accel.doubleTapINT(1);
  //   accel.singleTapINT(1);
}

void accelUpdate()
{
  debugMessage("Accelerometer data updated",1);
  staffCurrentMode = mMusic;
}

// void accelUpdate()
// {
//   int accelX, accelY, accelZ; // Current accelerometer measurements

//   // dump accelerometer values for comparison
//   prevAccelX = accelX;
//   prevAccelY = accelY;
//   prevAccelZ = accelZ;
  
//   // Accelerometer events to change MODE_XXX
//   //   MODE_NORMAL - (default post boot, first cycle position via z-axis rapid in and out
//   //   MODE_PAIR - drop quickly on y-axis
//   //   MODE_MUSIC - second cycle position via via z-axis rapid in and out
//   //   MODE_FLASHLIGHT - third cycle position via via z-axis rapid in and out
//   //   MODE_ERROR - N/A (automatic)

//   // Accelerometer events within MODE_XXX
//   //   MODE_NORMAL - ?
//   //   MODE_PAIR - ?
//   //   MODE_MUSIC - ?
//   //   MODE_FLASHLIGHT - none
//   //   MODE_ERROR - none (power reset required to leave MODE)
    
//   // Sparkfun accelerometer
//   // Review high level accelerometer events
//   byte interrupts = accel.getInterruptSource();
 
//   // Staff is double tapped
//   // if(accel.triggered(interrupts, ADXL345_DOUBLE_TAP))
//   // // Right now, the only way to activate MODE_FLASHLIGHT is through MODE_NORMAL
//   // {
//   //   if(staffPreviousMode = MODE_NORMAL)
//   //     {
//   //       debugMessage("Double tap switch from MODE_NORMAL to MODE_FLASHLIGHT");
//   //       staffCurrentMode = MODE_FLASHLIGHT;
//   //     }
//   //   else if(staffPreviousMode = MODE_FLASHLIGHT)
//   //     {
//   //       debugMessage("Double tap switch from MODE_FLASHLIGHT to MODE_NORMAL");
//   //       staffCurrentMode = MODE_NORMAL;
//   //     }
//   // }
  
//   // Activity
//   if(accel.triggered(interrupts, ADXL345_ACTIVITY))
//   {
//       debugMessage("x axis activity detected");
//   }
//   accel.readAccel(&accelX, &accelY, &accelZ);

//   // Inactivity
//   if(accel.triggered(interrupts, ADXL345_INACTIVITY))
//   {
//     debugMessage("no accelerometer activity");
//     //jump to the next sensor to check?
//   }
//   // Display the results (acceleration is measured in m/s^2)
//   debugMessage(String("X: "+accelX+" Y: "+accelY+" Z: "+accelZ+" m/s^2");

//   //Adafruit accelerometer
//   // sensors_event_t event; 
//   // accel.getEvent(&event);
//   // #ifdef DEBUG
//   //   // Display the results (acceleration is measured in m/s^2)
//   //   Serial.print("X: "); Serial.print(event.acceleration.x);
//   //   Serial.print(" Y: "); Serial.print(event.acceleration.y);
//   //   Serial.print(" Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2 ");
//   // #endif
// }


void staffUpdateNormal()
{
  debugMessage("MODE_NORMAL initiated",1);
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Blue);
  stripTwo.setOneColor(CRGB::Blue);
}

void staffUpdateFlashlight()
{
  debugMessage("MODE_FLASHLIGHT initiated",1);
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::White);
  stripTwo.setOneColor(CRGB::White);
}

void staffUpdatePair()
{
  debugMessage("MODE_PAIR initiated",1);
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Green);
  stripTwo.setOneColor(CRGB::Green);
}

void staffUpdateMusic()
{
  debugMessage("MODE_MUSIC initiated",1);
  FastLED.setBrightness(200);
  stripOne.setBreathe(CRGB::Green);
  stripTwo.setBreathe(CRGB::Green);
}

void staffUpdateError()
{
  debugMessage("MODE_ERROR intiatated",1);
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Red);
  stripOne.setOneColor(CRGB::Red);
}

void debugMessage(String messageText, uint8_t verbosityLevel)
// Description: wraps println() with increasing levels (verbosity) of debug message output
// Parameters:  debug message as String, 
//  verbosityLevel increases for more detailed debug information, currently 1 and 2 are used
// Output : NA (void)
// Improvement : ?
{
  #ifdef DEBUG
    if (verbosityLevel <= DEBUG)
    {
      Serial.println(messageText);
      Serial.flush();  // Make sure the message gets output (before any sleeping...)
    }
  #endif
}