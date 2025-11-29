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

// button support
#include <ezButton.h>
ezButton buttonOne(buttonPin,INPUT_PULLDOWN);

// global variables
uint32_t timeLastAccelSyncMS = 0;
uint32_t timeLastGPSSyncMS = 0;
enum staffModes staffCurrentMode;
uint8_t staffPreviousMode = mNormal; // forces change when mBoot is set
// int prevAccelX, prevAccelY, prevAccelZ; // last accelerometer measurements

void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial)
    debugMessage("wizardstaff p1 started",1);
  #endif

  ledInit();

  // show user that the staff is powering up
  staffChangeMode(mBoot);

  buttonOne.setDebounceTime(buttonDebounceDelayMS);


  // initialize accelorometer
  if (!accelInit())
  {
    debugMessage("Accelerometer failed to initialize",1);
    staffChangeMode(mError);
    deviceReset(5000);
  }

  // initialize GPS
  if (!gpsInit())
  {
    debugMessage("GPS failed to initialize",1);
    staffChangeMode(mError);
    deviceReset(5000);
  }

  staffChangeMode(mNormal);
}

void loop()
{
  buttonOne.loop();
  // check if buttons were pressed
  if (buttonOne.isReleased())
  {
    if (staffCurrentMode == mNormal)
      staffChangeMode(mFlashlight);
    else
      staffChangeMode(mNormal);
  }
  // // has the staff moved in a way to trigger a change?
  // if ((millis() - timeLastAccelSyncMS) > accelSampleIntervalMS) 
  // {
  //   accelUpdate();
  //   timeLastAccelSyncMS = millis();
  // }

  // // is it time to update the staff's GPS position?
  // if ((millis() - timeLastGPSSyncMS) > gpsSampleIntervalMS) 
  // {
  //   gpsUpdate();
  //   timeLastGPSSyncMS = millis();
  // }

  staffChangeMode(staffCurrentMode);
}

void staffChangeMode(uint8_t newMode)
{
  bool modeChange = false;  

  // do we need to change staff mode?
  if (newMode!=staffPreviousMode)
  {
    debugMessage(String("Staff mode was ") + staffPreviousMode + ", now " + newMode,2);
    staffPreviousMode = staffCurrentMode;
    staffCurrentMode = (enum staffModes)newMode;
    modeChange = true;
  }

  switch(staffCurrentMode)
  {
    case mBoot:
      staffModeBoot(modeChange);
      break;
    case mNormal:
      staffModeNormal(modeChange);
      break;
    case mFlashlight:
      staffModeFlashlight(modeChange);
      break;
    case mMusic:
      staffModeMusic(modeChange);
      break;
    case mPair:
      staffModePair(modeChange);
      break;
    case mError:
      staffModeError(modeChange);
      break;
  }

  //update LED strips
  stripOne.update();
  stripTwo.update();
  FastLED.show();
  delay(100);          // 10Hz clock for driving animations
}

void ledInit()
{
  FastLED.addLeds<WS2812B, ledStrip1DataPin, GRB>(ledsOne,ledStripPixelCount);
  FastLED.addLeds<WS2812B, ledStrip2DataPin, GRB>(ledsTwo,ledStripPixelCount);
  FastLED.setBrightness(200);
  debugMessage("ledInit() end",2);
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
// Description: 
// Parameters: NA
// Output: true if initialization successful
// Improvement: ?
{
  #ifdef HARDWARE_SIMULATE
    return true;
  #endif

  debugMessage("Accelerometer initialized",1);
  return true;
}

void accelUpdate()
// Description: 
// Parameters: NA
// Output: true if initialization successful
// Improvement: ?
{
  debugMessage("Accelerometer data updated",1);
  staffCurrentMode = mMusic;
  
  // Accelerometer events to change MODE_XXX
  //   MODE_NORMAL - (default post boot, first cycle position via z-axis rapid in and out
  //   MODE_PAIR - drop quickly on y-axis
  //   MODE_MUSIC - second cycle position via via z-axis rapid in and out
  //   MODE_FLASHLIGHT - third cycle position via via z-axis rapid in and out
  //   MODE_ERROR - N/A (automatic)
}

void staffModeBoot(bool modeChange)
// Description: LED patterns when wizardStaff is in boot mode
// Parameters: NA
// Output: NA
// Improvement: ?
{
  if (modeChange) {
    stripOne.setOneColor(CRGB::Yellow);
    stripTwo.setOneColor(CRGB::Yellow);
    debugMessage("staff LEDs in BOOT mode",1);
  }
  // otherwise no state to update
}

void staffModeNormal(bool modeChange)
{
  static uint8_t i, level_leds, progress;
  static uint32_t bitmap  = 0b1100110011001100; // marquee pattern
  static uint32_t counter = 0;

  if (modeChange) {
    counter = 0; // reset the state machine
    debugMessage("LEDs now in NORMAL mode",1);
  }

  // Cycle through patterns, with 64 updates for each
  if( (counter % 64) == 0) {
    switch((counter/64)%7) {
      case 0: {
        stripOne.setRunFwd(CRGB::Purple);
        stripTwo.setBreathe(CRGB::Orange);
        break;
      }
      case 1: {
        stripOne.setRunRev(CRGB::Red);
        stripTwo.setRunFwd(CRGB::Purple);
       break;
      }
      case 2: {
        stripOne.setRainbowRev();
        stripTwo.setRunRev(CRGB::Red);
        break;
      }
      case 3: {
        stripOne.setCylon(CRGB::Red);
        stripTwo.setRainbowRev();
        break;
      }
      case 4: {
        stripOne.setMarquee(CRGB::Yellow,bitmap);
        stripTwo.setCylon(CRGB::Red);
        break;
      }
      case 5: {
        stripOne.setOneColor(CRGB::Purple);
        stripTwo.setMarquee(CRGB::Yellow,bitmap);
        break;
      }
      case 6: {
        stripOne.setBreathe(CRGB::Orange);
        stripTwo.setOneColor(CRGB::Purple);
        break;
      }
      default: {
        stripOne.setOneColor(CRGB::Orange);
        stripTwo.setOneColor(CRGB::Orange);
        break;  // should never see this
      }
    }
  }
  counter++;
}

void staffModeFlashlight(bool modeChange)
{
  if (modeChange) {
    stripOne.setOneColor(CRGB::White);
    stripTwo.setOneColor(CRGB::White);
    debugMessage("LEDs now in FLASHLIGHT mode",1);
  }
  // otherwise no state to update
}

void staffModePair(bool modeChange)
{
  if (modeChange) {
    stripOne.setOneColor(CRGB::Green);
    stripTwo.setOneColor(CRGB::Green);
    debugMessage("staff LEDs in PAIR mode",1);
  }
  // otherwise no state to update
}

void staffModeMusic(bool modeChange)
{
  if (modeChange) {
    stripOne.setBreathe(CRGB::Green);
    stripTwo.setBreathe(CRGB::Green);
    debugMessage("staff LEDs in MUSIC mode",1);
  }
  // otherwise no state to update
}

void staffModeError(bool modeChange)
{
  if (modeChange) {
    stripOne.setOneColor(CRGB::Red);
    stripOne.setOneColor(CRGB::Red);
    debugMessage("staff LEDs now in ERROR mode",1);
  }
  // otherwise no state to update
}

void deviceReset(uint16_t ledTimerMS)
// Description: resets SAMD21 based device
// Parameters:  
// Output : NA (void)
// Improvement : ?
{
  // allow LED visualization time to alert user
  delay(ledTimerMS);

  debugMessage("resetting wizardStaff",1);

  // Disable all interrupts to prevent interruption during reset
  __disable_irq();

  delay(100);
  NVIC_SystemReset();

  // wait indefinitely to ensure the reset completes
  while (true);
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