/*
  Project Name:   wizardstaff proto 1
  Description:    public (non-secret) configuration data
*/	

// Configuration Step 1: Set debug message output
// comment out to turn off; 1 = summary, 2 = verbose
#define DEBUG 1

// Configuration Step 2: simulate WiFi and sensor hardware,
// returning random but plausible values
// comment out to turn off
// #define HARDWARE_SIMULATE

// Configuration variables that change rarely

const uint8_t ledStrip1DataPin = 11;
const uint8_t ledStrip2DataPin = 12;
const uint8_t ledStripPixelCount = 17; // number of LEDs on each strip

// Buttons
const uint8_t buttonPin = 10; // initially LOW
const uint16_t buttonDebounceDelayMS = 50; // time to debounce button


// Wizardstaff modes
enum staffModes {mBoot, mNormal, mPair, mMusic, mFlashlight, mError};

const uint32_t gpsSampleIntervalMS = 90000;
const uint32_t accelSampleIntervalMS = 60000;