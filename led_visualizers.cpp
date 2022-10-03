#include "Arduino.h"

// hardware and internet configuration parameters
#include "config.h"

// Shared helper function we call here too...
extern void debugMessage(String messageText);

void staffModeNormalUpdate()
{
  debugMessage("MODE_NORMAL initiated");
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Blue);
  stripTwo.setOneColor(CRGB::Blue);
}

void staffModeFlashlightUpdate()
{
  debugMessage("MODE_FLASHLIGHT initiated");
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::White);
  stripTwo.setOneColor(CRGB::White);
}

void staffModePairUpdate()
{
  debugMessage("MODE_PAIR initiated");
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Green);
  stripTwo.setOneColor(CRGB::Green);
}

void staffModeMusicUpdate()
{
  debugMessage("MODE_MUSIC initiated");
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Yellow);
  stripTwo.setOneColor(CRGB::Yellow);
}

void staffModeErrorUpdate()
{
  debugMessage("MODE_ERROR intiatated");
  FastLED.setBrightness(200);
  stripOne.setOneColor(CRGB::Red);
  stripOne.setOneColor(CRGB::Red);
}