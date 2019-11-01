# Wizardstaff - Prototype 1
## What is a wizardstaff? 
If you've been to Burning Man or EDC with friends, you know it's hard to find people over hundreds of acres and hundreds of thousands of people. Imagine if you were carrying a staff that reacts to the music via cool LEDs, functions as the biggest flashlight you've ever owned, and most importantly, helped you find your lost soul mate. Wizardstaffs can guide their owners to each other other over many miles using GPS, 900mhz radios, and those LEDs we previously mentioned.

### Proto1 Purposes
  - Explore the best way to wire LEDs for wizardstaff
  - Explore different 5v battery options for the base
  - Prototype to handle regularly to discover use patterns
  - Prototype accelerometer based events and associated LED effects
   
### Contributors
@disquisitioner builds [LED Control](https://github.com/disquisitioner/Arduino/tree/master/LED%20Projects/LED_Control).

### BOM (Bill of Materials)
  - 1X: 23" PVC pipe (staff core)
  - 1X: Adafruit LPD8086 LED strip (data and clock pins, 5V, ground), 18 LEDs (part 306)
  - 1X: Adafruit Feather M0 RFM69HCW (part 1376)
  - 1X: Adafruit ADXL 345 Accelerometer (part 1231)
  - 2X: Adafruit 2 pin JST male/female connectors (part 2880)
  - 1X: narrow protoboard
  - wire
  - Dicor BT-1834-1 1/8" x 3/4" x 30' Butyl Seal Tape

### Learnings
  - need to write up build process before building
  - fritzing diagrams
  - take pictures of build during process
  - branch power at staff top for four strips and MCU board immediately. I forgot on P1 and now will likely have to power MCU via LiPo on top
  - each LED strip needs GND connection to MCU board.
  - need plenty of 2pin JST connectors!
  - didn't think I would need a protoboard but I do for GND routing, accelerometer. Will need a place at the top of P2 for this between the LED strips
  - ideally create a simple PCB to route 5V power, GND from base to circular 90degree endpoints around staff plus central tap for MCU
 
### Issues
  - Adafruit Feather might have issues with 5V to VBUS, GND (according to Adafruit, this is not recommended?)
  - 6/9/19: ran out of Adafruit 318/319, replaced by 2880, ordered many 318/319 for P2
  - need to look at larger, custom 5V batteries for P2
  - see build photos for discovered p2 issues
    
### Proto2 build gates
  - 2+hours P1 freeform testing
  - parts on-hand for build
  - determination of P2 battery solution
  - test powering Feather M0 using 5v from battery

### Information Sources
#### LED strips
- https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels
- https://learn.adafruit.com/battery-power-for-led-pixels-and-strips/about-batteries
#### Accelerometer
- https://howtomechatronics.com/tutorials/arduino/how-to-track-orientation-with-arduino-apnd-adxl345-accelerometer/
- https://learn.sparkfun.com/tutorials/adxl345-hookup-guide/all
- https://forums.adafruit.com/viewtopic.php?f=25&t=51029
- https://learn.adafruit.com/adxl343-breakout-learning-guide?view=all
##### Accelerometer via circuit python *(not used in proto1)*
- https://learn.adafruit.com/make-it-shake-rattle-and-roll?view=all
- https://www.analog.com/en/analog-dialogue/articles/detecting-falls-3-axis-digital-accelerometer.html

### Revisions *(independent of check-ins)*
  - 06/07/19: [FR 06/02/19] fork from lightbar.ino
  - 06/07/19: [FR 11/24/17] implement DEBUG flag for print and println and put !Serial under that debug flag
  - 09/03/19: Moved all P1 .plan information to this file
    Added accelerometer basic read
  - 09/14/19: integrated David Bryant's LEDControl library to replace lightfieldeffects
  - 09/14/19: [FR 06/09/19] verify WS2812B ( https://www.adafruit.com/product/1376?length=1 )
  - 09/14/19: [FR 11/20/17] enum stripColor for readability, closed due to LEDControl implemetation
    
### Feature Requests
  - 05/07/19: error handling review