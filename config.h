/*
  Project Name:   wizardstaff proto 1
  Description:    public (non-secret) configuration data
*/	

// Configuration Step 1: Set debug message output
// comment out to turn off; 1 = summary, 2 = verbose
#define DEBUG 2

// Configuration Step 2: simulate WiFi and sensor hardware,
// returning random but plausible values
// comment out to turn off
// #define HARDWARE_SIMULATE

// Configuration variables that change rarely

const uint8_t ledStrip1DataPin = 11;
const uint8_t ledStrip2DataPin = 12;
const uint8_t ledStripPixelCount = 17; // number of LEDs on each strip

// Wizardstaff modes
enum staffModes {mNormal, mPair, mMusic, mFlashlight, mError};

const uint32_t gpsSampleIntervalMS = 15000;
const uint32_t accelSampleIntervalMS = 10000;