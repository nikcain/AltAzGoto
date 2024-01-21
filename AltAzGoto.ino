// modules:
// stepper moves
// input commands
// look up coordinates

// required to keep program space usage low
#define DECODE_NEC 

#include "Arduino.h"
#include "keymapping.h"
#include "LiquidCrystal.h"
#include "devicetime.h"
#include "CelestialDatabase.h"
#include "IRremote.h"
#include "motors.h"

//                RS E  D4 D5  D6  D7
LiquidCrystal lcd(10, 9, 7,6,5,4);//4, 5, 6, 7);
IRrecv irrecv(8);

int currentAction;
CelestialDatabase cdb;
String celestialObjectID = "";
CelestialGotoObject targetObject;
AltAzPosition targetPosition;
bool calibrating = false;
stepperMotors motors;

// status codes
#define INACTIVE -1
#define TRACKING 16
//celestialObjectID complete, ready to look up
#define LOOKUP 20 
//object found, waiting confirmation to slew
#define READYTOGO 21
#define SLEWING 22
#define SETTIME 23
#define MOVING 24
#define PACKAWAY 26

// image for play arrow
const byte rarrow[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
};

int updown = 0;
int leftright = 0;
int datetimeitembeingedited = 0;

void setup() {  
  Serial.begin(9600);
  //Serial.println("lets go!");
  lcd.begin(16, 2);
  lcd.createChar(0, rarrow);
  irrecv.enableIRIn();
  calibrating = false;
  motors.init();
  lcd.noCursor();
  lcd.noBlink();
}

int getNumberKey(int cmd)
{
  if (cmd == key_0) return 0;
  if (cmd == key_1) return 1;
  if (cmd == key_2) return 2;
  if (cmd == key_3) return 3;
  if (cmd == key_4) return 4;
  if (cmd == key_5) return 5;
  if (cmd == key_6) return 6;
  if (cmd == key_7) return 7;
  if (cmd == key_8) return 8;
  if (cmd == key_9) return 9;
  return -1; 
}

void loop() {
  while (irrecv.decode()) {
    //Serial.println(irrecv.decodedIRData.command);
    int cmd = irrecv.decodedIRData.command;
    switch (cmd) {
      case key_play:
        lcd.clear();
        if (currentAction == LOOKUP)
        {
          targetPosition = targetObject.getCurrentAltAzPosition(getYear(), getMonth(), getDay(), getHour(), getMinute());
          //Serial.println("set target");
          lcd.println(targetPosition.alt,5);
          //Serial.println(targetPosition.az,5);
          motors.setTarget(targetPosition.alt, targetPosition.az);
          currentAction = SLEWING;
        }
        else {
          lcd.print((currentAction == TRACKING) ? "stopped" : "tracking");
          currentAction = (currentAction == TRACKING) ? INACTIVE : TRACKING;
        }
        break;
      case key_up:
      case key_down:
      case key_left:
      case key_right:
        updown = (cmd == key_up) ? 1 : (cmd == key_down) ? -1 : 0;
        leftright = (cmd == key_left) ? -1 : (cmd == key_right) ? 1 : 0;
        if (currentAction != SETTIME) {
          motors.Move(updown,leftright,0,calibrating); // alt az cal
          currentAction = INACTIVE;
        }
        break;
      case key_back:
        // delete celestialObjectID character
        if (celestialObjectID.length() > 0) {
          celestialObjectID = celestialObjectID.substring(0,celestialObjectID.length()-1);
          lcd.clear();
          lcd.print(celestialObjectID);
        }
        break;
      case key_menu:
        datetimeitembeingedited = 0;
        currentAction = SETTIME;
        lcd.clear();
        break;
      case key_calibrate:
        if (calibrating) {
          calibrating = false;
          lcd.clear();
          lcd.print("Calibrating mode");
          lcd.setCursor(6,1);
          lcd.print("off");
        }
        else {
          calibrating = true;
          lcd.clear();
          lcd.print("Calibrating mode");
          lcd.setCursor(7,1);
          lcd.print("on");
        }
        break;
      case key_power:
        // sets position to vertical up, az pointing north
        currentAction = PACKAWAY;
        break;
      default:
        if (getNumberKey(irrecv.decodedIRData.command) > -1) 
            {
              celestialObjectID.concat(getNumberKey(irrecv.decodedIRData.command));
              lcd.clear();
              lcd.noCursor();
              lcd.print(celestialObjectID);
              if (celestialObjectID.length() == 4) {
                targetObject.id = celestialObjectID.toInt();
                if (cdb.FindCelestialGotoObject(&targetObject)) {
                  targetPosition = targetObject.getCurrentAltAzPosition(getYear(), getMonth(), getDay(), getHour(), getMinute() +getSeconds()/60.0); 
                  
                  lcd.clear();
                  if (!targetObject.isAboveHorizon(getYear(), getMonth(), getDay(), getHour(), getMinute()))
                  {
                    lcd.print("error - ");
                    lcd.print(targetObject.name);
                    lcd.setCursor(0,1);
                    lcd.print("below horizon");
                    currentAction = INACTIVE;
                  }
                  else {
                    lcd.print(targetObject.name);
                    lcd.setCursor(0,1);
                    lcd.write(byte(0));
                    lcd.print(" to go");
                    currentAction = LOOKUP;
                  }
                }
                else
                {
                  lcd.print("error finding");
                  lcd.setCursor(0,1);
                  lcd.print("object " + celestialObjectID);
                  currentAction = INACTIVE;
                } 
                celestialObjectID = "";
              }
            }
        break;
    }
    delay(200);
    irrecv.resume();
  }

  // state machine actions
  switch (currentAction) {
    case SLEWING:
      // slewing to object
      if (motors.completedSlew()) currentAction = INACTIVE;
      break;
    // manual move. Values for direction, not steps (motors will handle amounts)
    case TRACKING:
      // tracking
      targetPosition = targetObject.getCurrentAltAzPosition(getYear(), getMonth(), getDay(), getHour(), getMinute() + getSeconds()/60.0);
      motors.setTarget(targetPosition.alt, targetPosition.az);
      break;
    case SETTIME:
        if (updown != 0) {
          setDeviceTime(getYear()+(updown * ((datetimeitembeingedited == 2)? 1:0)), 
                        getMonth()+(updown * ((datetimeitembeingedited == 1)? 1:0)), 
                        getDay()+(updown * ((datetimeitembeingedited == 0)? 1:0)),
                        getHour()+(updown * ((datetimeitembeingedited == 3)? 1:0)), 
                        getMinute()+(updown * ((datetimeitembeingedited == 4)? 1:0)), 
                        getSeconds()+(updown * ((datetimeitembeingedited == 5)? 1:0)));
          updown = 0;
        }
        if (leftright != 0) {
          datetimeitembeingedited += leftright;
          if (datetimeitembeingedited < 0) datetimeitembeingedited = 0;
          if (datetimeitembeingedited > 5) { lcd.noCursor(); lcd.clear(); currentAction = INACTIVE; break; }
          leftright = 0;
        }

        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print(getDateString());
        lcd.setCursor(0,1);
        lcd.print(getTimeString());
        lcd.setCursor(1+datetimeitembeingedited % 3 * 3,(datetimeitembeingedited > 2)?1:0);
        lcd.cursor();
        delay(200);
        break;
    case PACKAWAY:
        motors.setTarget(90,0);
    break;
    default:
      break;
  }
  motors.update();
}

