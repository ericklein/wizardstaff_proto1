// conditional compile flags
#define DEBUG 	// Output to serial port

#define ledStrip1DataPin  11
#define ledStrip2DataPin  12
#define LEDS_STRIP        17 // number of LEDs on each strip

// Wizardstaff modes
#define MODE_NORMAL     0 // rotating animations
#define MODE_FIND       1 // find the matching wizardstaff
#define MODE_MUSIC      2 // reacts to microphone audio
#define MODE_FLASHLIGHT 3 // lots of light
#define MODE_ERROR      4 // alert user to error condition

#define GPS_UPDATE_INTERVAL 30000   // milliseconds between polling for update
#define ACCEL_UPDATE_INTERVAL 1000  // milliseconds between polling for update