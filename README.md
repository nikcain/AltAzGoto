# AltAzGoto

This is a work in progress!

This is Arduino code to select a celestial object and automatically move the telescope to point to it.
Hardware required is:
* arduino UNO board
* two 2phase stepper motors
* two DRV8825 stepper drivers
* arduino SD-card module
* LCD display
* IR remote and receiver

Current status of code: user can set time on board, enter an ID of object, select to slew, select to move, select to track, and current Alt-Az coordinates calculated for both stars and planets.

Todo: 
* Implement list of celestial objects on SD card
* drive stepper motors:
  * slewing
  * tracking
  * manual moving
