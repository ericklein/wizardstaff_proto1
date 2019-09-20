#include "Arduino.h"
#include <FastLED.h>
#include "LEDControl.h"

// Constructor class, mostly just saves key attributes
LEDControl::LEDControl(int pin, int num_leds, CRGB leds[])
{
  _pin = pin;
  _ledCount = num_leds;
  _leds = leds;
  _prevMode = MODE_UNDEF;
  _mode = MODE_OFF;
  _color = CRGB::Black;
  _config = 0;
}

// Returns the current operating mode (see LEDControl.h for values)
int LEDControl::getMode()
{
  return _mode;
}

// All LEDs on, set to the same solid color
void LEDControl::setOneColor(CRGB color)
{
  _prevMode = _mode;
  _mode = MODE_ON;
  _color = color;
}

// Sequences a single color from the beginning of the strip to the end.
// Wraps around from end to beginning if left on long enough
void LEDControl::setRunFwd(CRGB color)
{
  _prevMode = _mode;
  _mode = MODE_RUNFWD;
  _color = color;
}

// Sequences a single color from the end of the strip back to the 
// beginning ("reverse"). Wraps around from beginning back to end
// if left on long enough
void LEDControl::setRunRev(CRGB color)
{
  _prevMode = _mode;
  _mode = MODE_RUNREV;
  _color = color;
}

// Loads the strip with a rainbow and then runs it forward
void LEDControl::setRainbowFwd()
{
  _prevMode = _mode;
  _mode = MODE_RAINBF;
}

// Loads the strip with a rainbow and then runs it in reverse
void LEDControl::setRainbowRev()
{
  _prevMode = _mode;
  _mode = MODE_RAINBR;
}

// Runs a color back and forth (a la a Cylon's red eye)
void LEDControl::setCylon(CRGB color)
{
  _prevMode = _mode;
  _mode = MODE_CYLON;
  _color = color;
}



// Master update function, called once per strip each clock cycle to
// do whatever is needed to sequence the strip ahead by one 'tick'.
// Uses the current mode of the strip to figure out what to do.
void LEDControl::update()
{
  int delta;
  
  switch(_mode) {
    case MODE_UNDEF:
      break; // Shouldn't happen, but whatever
    
    case MODE_OFF:
      if(_prevMode != MODE_OFF) {
        // Turn LEDs all off
        fill_solid(_leds,_ledCount,CRGB::Black);
        _prevMode = MODE_OFF;
      }
      break;
    
    case MODE_ON:
      if(_prevMode != MODE_ON) {
        fill_solid(_leds,_ledCount,_color);
        _prevMode = MODE_ON;
      }
      break;
    
    case MODE_RUNFWD:
      // If we're changing modes, light just the first LED
      if(_prevMode != MODE_RUNFWD) {
        fill_solid(_leds,_ledCount,CRGB::Black);  // May not want to do this
        _leds[0] = _color;
        _prevMode = MODE_RUNFWD;
      }
      else {
        // if we're not changing modes, move the LED one down the strip
        // (with rollover of last LED color to first LED
        shiftFwd();
      }
      break;
    
    case MODE_RUNREV:
      // If we're changing modes, light just the last LED
      if(_prevMode != MODE_RUNREV) {
        fill_solid(_leds,_ledCount,CRGB::Black);  // May not want to do this
        _leds[_ledCount-1] = _color;
        _prevMode = MODE_RUNREV;
      }
      else {
        // if we're not changing modes, move the LED one up the strip
        // (with rollover of first LED color to last LED
        shiftRev();  // use the convenience function
      }
      break;
    
    case MODE_RAINBF:
      // Just need to initialize the strip with a rainbow pattern
      // then run it as MODE_RUNFWD
      delta = 256 / _ledCount;
      if(_prevMode != MODE_RAINBF) {
        // Load up the initial rainbow
        for(int i=0;i<_ledCount;i++) {
          _leds[i] = CHSV(i*delta,255,255);
        }
        _prevMode = MODE_RUNFWD;
        _mode = MODE_RUNFWD;
      }
      else {
        // Should never get here!
        Serial.println("Rainbow fwd error");
      }
      break;
    
    case MODE_RAINBR:
      // Just need to initialize the strip with a rainbow pattern
      // then run it as MODE_RUNREV
      delta = 256 / _ledCount;
      if(_prevMode != MODE_RAINBR) {
        // Load up the initial rainbow
        for(int i=0;i<_ledCount;i++) {
          _leds[i] = CHSV(i*delta,255,255);
        }
        _prevMode = MODE_RUNREV;
        _mode = MODE_RUNREV;
      }
      else {
        // Should never get here!
        Serial.println("Rainbow rev error");
      }
      break;

    // Implement alternating forward & reverse runs.  Is careful to have
    // a full cycle time equal to 2x the number of LEDs so it can stay in
    // synch with regular single direction runs.
    case MODE_CYLON:
      if(_prevMode != MODE_CYLON) {
        fill_solid(_leds,_ledCount,CRGB::Black);  // May not want to do this
        _leds[0] = _color;
        _config = MODE_RUNFWD;  // Need to keep track of movement direction
        _prevMode = MODE_CYLON;        
      }
      else {
        if(_config == MODE_RUNFWD)  {
          // Moving the LED forward
          // Are we at the end?
          if(_leds[_ledCount-1] == _color){
            _config = MODE_RUNREV;
          }
          else {
            shiftFwd();
          }
        }
        else {
          // Moving the LED reverse
          if(_leds[0] == _color) {
            _config = MODE_RUNFWD;
          }
          else {
            shiftRev();
          }
        }
      }
      break;
    
    default:
      Serial.println("Unrecognized mode");
      break;
  }
}

void LEDControl::shiftFwd()
{
  CRGB last = _leds[_ledCount-1];
  for(int i=_ledCount-1;i>0;i--) {
    _leds[i] = _leds[i-1];
  }
  _leds[0] = last;
}

void LEDControl::shiftRev()
{
  CRGB first = _leds[0];
  for(int i=0;i<_ledCount-1;i++) {
    _leds[i] = _leds[i+1];
  }
  _leds[_ledCount-1] = first;
}
