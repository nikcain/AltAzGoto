# AltAzGoto

I'm going to retire this project in favour of using an ESP32 processor instead. The advantages will be that I can network it, which will mean streamlining the code quite a lot, effectively allowing it to setting the date and time automatically, having the star & DSO catalog off-board, allow planetary calculations to also take place elsewhere, and no need for the LCD display (maybe). The new repository is here: https://github.com/nikcain/AltAzGoto-ESP32


This is a work in progress! Wokwi version is here: https://wokwi.com/projects/385207165869856769 

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

The database of celestial objects contains all NGC objects, about 450 major stars, and planets (incuding pluto, not that you'll be able to see it). The database uses the NGC number as the key, so if you enter 0224, you get NGC 224 (andromeda galaxy). NGC catalogue is 0001 to 7840, the planets are 9991 to 9999 (i.e. 9994 is mars), and major stars are 8000 to 8450. I've tried to add common names where possible (looks like there's bug when I generated the db as the star names didn't make it!).

Current status of code: functionally complete, and testing on-going. 
pin|use||pin|use
---|---|---|---|---
0|||IOREF|test
1|||RESET|
2|||3.3V|
3|||5V|
4|LCD D7||GND|
5|LCD D6||GND|
6|LCD D5||Vin|
|7|LCD D4||A0|Az step
8|IRrecv||A1|Az dir
9|LCD E||A2|Alt step
|10|LCD RS||A3|Alt dir
11|SDcard MOSI||A4|
12|SDcard MISO||A5|
|13|SDcard SCK|||
|GND|SDcard CS|||
|AREF|||
|SDA|||
|SCL|||

Todo: 
* bug test
* bug test
* bug test
* slew so focusser is always facing up
* track without doing crazy 180 degree spins of the azimuth

Current status of hardware - I've come to realise how much gearing is required to get sub-arcsecond tracking, and without buying some more stuff it looks like I'll be settling for decent visual tracking, and leave astrophotography for another day....
