#ifndef LEDControl_h
#define LEDControl_h

// Display modes for the LEDs
#define MODE_UNDEF  0
#define MODE_OFF    1
#define MODE_ON     2
#define MODE_RUNFWD 3
#define MODE_RUNREV 4
#define MODE_RAINBF 5
#define MODE_RAINBR 6
#define MODE_CYLON  7
#define NUM_MODES   8

#include "Arduino.h"
class LEDControl
{
  public:
    LEDControl(int pin, int num_leds, CRGB leds[]);
    int getMode();
    void setOneColor(CRGB color);
    void setRunFwd(CRGB color);
    void setRunRev(CRGB color);
    void setCylon(CRGB color);
    void setRainbowFwd();
    void setRainbowRev();
    void shiftFwd();
    void shiftRev();
    void update();
  private:
    int _pin;
    int _ledCount;
    CRGB *_leds;
    int _prevMode;
    int _mode;
    CRGB _color;
    int _config;
};

#endif
