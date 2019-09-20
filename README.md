# wizardstaff_proto1
Staff for concerts and raving

proto1 purposes
  Explore the best way to wire LEDs to "staff"
  Explore different 5v battery options for the base
  Prototype to handle on a regular basis for use pattern discovery
  Test and prototype accelerometer based events and LED patterns 
   
proto1 BOM
  1X: ~2ft long PVC pipe as staff
  1X: Adafruit LPD8086 LED strip (data and clock pins, 5V, ground), 18 LEDs (part 306)
  1X: Adafruit Feather M0 RFM69HCW (part 1376)
  1X: Adafruit ADXL 345 Accelerometer (part 1231)
  2X: Adafruit 2 pin JST male/female connectors (part 2880)
  1X: narrow protoboard
  wire
   
proto1 software dependencies
  Adafruit Sensor library
  Adafruit ADXL345 library
  LEDControl library from David Bryant
  FASTLED

Learnings
  - build process
  - branch power at staff top for four strips and MCU board immediately. I forgot on P1 and now will likely have to power MCU via LiPo on top
  - each LED strip needs GND connection to MCU board.
  - need plenty of 2pin JST connectors!
  - didn't think I would need a protoboard but I do for GND routing, accelerometer. Will need a place at the top of P2 for this between the LED strips
  - ideally create a simple PCB to route 5V power, GND from base to circular 90degree endpoints around staff plus central tap for MCU
 
Issues?
  - Adafruit Feather might have issues with 5V to VBUS, GND (according to Adafruit, this is not recommended?)
  - Feather is 3.3v, look at for integration with ADXL 345
  - 6/9/19 - waiting for Adafruit 3784 (4xAA battery holder with a 2.1mm plug)
  - 6/9/19 - ran out of Adafruit 318/319, replaced by 2880, ordered many units for P2
  - 6/9/19 - Both of my Adafruit Feather M0 RFM69HCW (part 1376) are committed to P1 and P2, order 2 more for radio unit tests (currently out of stock)
  - many ways to test battery/powers options for P1 on order from Adafruit, gates P2
  - need to look at larger, custom 5V batteries for P2
    
proto2 build gates
  2+hours P1 freeform testing
  missing parts for the build
  determination of P2 battery solution
  test powering Feather M0 using 5v from battery

Information Sources
LED strips
https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels
https://learn.adafruit.com/battery-power-for-led-pixels-and-strips/about-batteries
Accelerometer
https://howtomechatronics.com/tutorials/arduino/how-to-track-orientation-with-arduino-apnd-adxl345-accelerometer/
https://learn.sparkfun.com/tutorials/adxl345-hookup-guide/all
https://forums.adafruit.com/viewtopic.php?f=25&t=51029
https://learn.adafruit.com/adxl343-breakout-learning-guide?view=all
https://learn.adafruit.com/make-it-shake-rattle-and-roll?view=all
https://www.analog.com/en/analog-dialogue/articles/detecting-falls-3-axis-digital-accelerometer.html

Revisions
  - 06/07/19
    [FR 06/02/19] multiple strip support; fork from lightbar.ino
    [FR 11/24/17] implement DEBUG flag for print and println and put !Serial under that debug flag
  - 09/03/19
    Moved all P1 .plan information to this file
    Added accelerometer basic read
  -09/14/19
    integrated David Bryant's LEDControl library to replace lightfieldeffects
    [FR 06/09/19] verify WS2812B ( https://www.adafruit.com/product/1376?length=1 )
    [FR 11/20/17] enum stripColor for readability - closed due to LEDControl implemetation
    
Feature Requests
  - 05/07/19
    - error handling review