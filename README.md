# AltAzGoto

This is a work in progress!

This is Arduino code to select a celestial object and automatically move the telescope to point to it.
Hardware required is:
* arduino UNO board
* two 2phase stepper motors (in my case nema17 42A02C)
* two DRV8825 stepper drivers + driver extension shields
* arduino SD-card module + 2GB SD card
* LCD display
* potentiometer to adjust LCD brightness
* IR remote and receiver
* microswitch to prevent slewing past horizon

The database of celestial objects contains all NGC objects, about 450 major stars, and planets (incuding pluto, not that you'll be able to see it).

Current status of code: functionally complete, but untested. Program space is just squeezing onto an arduino Uno R3 (97%), so not much wriggle room left to add more functionality, unless I relent and buy a arduino mega for a couple more quid!

Todo: 
* match motor parameters to real life gears
* bug test
* bug test
* bug test
* slew so focusser is always facing up
* track without doing crazy 180 degree spins of the azimuth

Current status of hardware - I've come to realise how much gearing is required to get sub-arcsecond tracking, and without buying some more stuff it looks like I'll be settling for decent visual tracking, and leave astrophotography for another day....
