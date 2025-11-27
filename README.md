# Wizardstaff - Prototype 1
last updated 27-Nov, 2025
## Purpose 
If you've been to Burning Man or EDC, you know it's hard to find friends when the festival has 100K+ attendees scattered over acres if not miles. Imagine carrying a staff that reacts to the music via beautiful LED patterns, functions as the biggest flashlight you've ever owned, and most importantly, helps you find your lost soul mate. Paired wizardstaffs guide their owners to each other other! Oh yeah, they are also a good totem base :)

## Features
TODO
   
### Software Dependencies
[disquisitioner](https://github.com/disquisitioner) builds [LED Control](https://github.com/disquisitioner/Arduino/tree/master/LED%20Projects/LED_Control).

## BOM (Bill of Materials)
### MCU
[Adafruit Feather M0 RFM69HCW](https://www.adafruit.com/product/3176)
  - 915MHz radio for inter-staff communication, see .plan
### Sensors
  - testing, TBD
### LEDs
2x WS2812b 5V LED strips
  - 17 LEDs per strip
  - ~ 2A max power draw
### Buttons/Switches
none
  - single button might be added in P2 depending on 9DOF gesture testing in P1
### Frame/Etc.
  - 23" PVC pipe, aka barrel frame
  - 1X: narrow protoboard
  - wire
  - Dicor BT-1834-1 1/8" x 3/4" x 30' Butyl Seal Tape

## Pinouts
TODO

## Issues/Feature Requests
See GitHub issues for issues and feature requests not covered in .plan

## .plan
### Prototype 1
  - Controlling 2 WS2812B LED strips with independent and linked patterns (LED Control)
  - How to attach and wire LED strips in the barrel frame
  - How to store and wire battery (5V) in the barrel frame 
  - Discover use patterns and usability via regular use
  - Test 9DOF gesture recognition to change staff modes
#### Prototype 1 exit criteria
  - 2+ hours P1 freeform testing by three people
  - P2 build parts on-site
  - P2 battery solution documented
### Prototype 2
  - switch to 1 meter staff?
  - Expand to 4 WS2812b LED strips
    - if larger staff, expand to 29 LEDs/strip
      - 20mA per LED min, 60mA max
      - min total draw ~2.4A, max ~ 7.2A
  - GPS integration
  - custom power distribution board
  - is a button needed?
    - reset
    - mode change, dependent on 9DOF gesture recognition testing in P1
### Prototype 3
  - switch to 1 meter staff if not completed in P2
  - Transparent barrel frame
  - LEDs inside staff via spine
  - 915Mhz communication between staffs to guide owners to each other

## Supporting Material
Mechanical components
- barrel
- bottom cap 
- battery assembly
- spine assembly
- brain assembly
- top cap

## Prototype 1 Learnings
- 03-Jul, 2017
  - Helens says the diameter is too wide
    - look at baseball bat or lacrosse stick as guides
      - balance is an issue
  - want it to freed stand on floor
  - needs a sling to carry it around for awhile
  - attach to a water pack easily?
  - need to leave the balance point open to carry it easily
  - needs balanced weight
  - telescoping?
  - soft top to push down on
  - hard, tough bottom to deal with the ground
- 06-Jun, 2019
  - need to write up build process before building
  - fritzing diagrams
  - take pictures of build during process
  - branch power at staff top for four strips and MCU board immediately. I forgot on P1 and now will likely have to power MCU via LiPo on top
  - each LED strip needs GND connection to MCU board.
  - need plenty of 2pin JST connectors!
  - didn't think I would need a protoboard but I do for GND routing, accelerometer. Will need a place at the top of P2 for this between the LED strips
  - ideally create a simple PCB to route 5V power, GND from base to circular 90degree endpoints around staff plus central tap for MCU