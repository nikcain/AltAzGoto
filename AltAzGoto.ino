// modules:
// stepper moves
// input commands
// look up coordinates

#include "Arduino.h"
#include "keymapping.h"
#include "LiquidCrystal.h"
#include "devicetime.h"
#include "CelestialDatabase.h"
#include "IRremote.h"
#include "motors.h"

//                RS E  D4 D5  D6  D7
LiquidCrystal lcd(5, 6, 7, 8, 9, 10);
IRrecv irrecv(4);

int currentAction;
CelestialDatabase cdb;
String celestialObjectID = "";
CelestialGotoObject targetObject;
AltAzPosition targetPosition;
AltAzPosition currentPosition;
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
#define SETHOUR 23
#define SETMINUTE 24
#define SETSECONDS 25

// image for play arrow
byte rarrow[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
};

void setup() {
  Serial.begin(115200);
  currentPosition.alt = 0.0;
  currentPosition.az = 0.0;
  targetPosition.alt = 0.0;
  targetPosition.az = 0.0;
  lcd.begin(16, 2);
  lcd.createChar(0, rarrow);
  irrecv.enableIRIn();
  calibrating = false;


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
    switch (irrecv.decodedIRData.command) {
      case key_play:
        if (currentAction == LOOKUP)
        {
          targetPosition = targetObject.getCurrentAltAzPosition(getHour(), getMinute());
          motors.setTarget(targetPosition.alt, targetPosition.az);
          currentAction = SLEWING;
        }
        else {
          lcd.clear();
          lcd.print((currentAction == TRACKING) ? "stopped" : "tracking");
          currentAction = (currentAction == TRACKING) ? INACTIVE : TRACKING;
        }
        break;
      case key_up:
        if (currentAction == SETHOUR) {
            setDeviceTime(getHour()+1, getMinute(), getSeconds());
            break;
          }

        if (currentAction == SETMINUTE) {
            setDeviceTime(getHour(), getMinute()+1, getSeconds());
            break;
          }

        if (currentAction == SETSECONDS) {
            setDeviceTime(getHour(), getMinute(), getSeconds()+1);
            break;
          }
      case key_down:
        if (currentAction == SETHOUR) {
            setDeviceTime(getHour()-1, getMinute(), getSeconds());
            break;
          }
          if (currentAction == SETMINUTE) {
            setDeviceTime(getHour(), getMinute()-1, getSeconds());
            break;
          }
          if (currentAction == SETSECONDS) {
            setDeviceTime(getHour(), getMinute(), getSeconds()-1);
            break;
          }
      case key_left:
        if (currentAction == SETHOUR) { currentAction = -1; lcd.clear(); break; }
        if (currentAction == SETMINUTE || currentAction == SETSECONDS) {
          currentAction--;
          break;
        }
      case key_right:
        if (currentAction == SETSECONDS) { currentAction = -1; lcd.clear(); break; }
        if (currentAction == SETHOUR || currentAction == SETMINUTE) {
          currentAction++;
          break;
        }
        lcd.clear();
        lcd.print("moving");
        currentAction = irrecv.decodedIRData.command;
        break;
      case key_back:
        // delete celestialObjectID character
        if (celestialObjectID.length() > 0) celestialObjectID = celestialObjectID.substring(0,celestialObjectID.length()-1);
        lcd.clear();
        lcd.print(celestialObjectID);
        break;
      case key_menu:
        lcd.clear();
        lcd.print(getTimeString());
        currentAction = SETHOUR;
        break;
      case key_calibrate:
        if (calibrating) {
          calibrating = false;
          lcd.clear();
          lcd.print("Calibrating mode off");
        }
        else {
          calibrating = true;
          lcd.clear();
          lcd.print("Calibrating mode on");
        }
        break;
      default:
        if (getNumberKey(irrecv.decodedIRData.command) > -1) 
            {
              celestialObjectID.concat(getNumberKey(irrecv.decodedIRData.command));
              lcd.clear();
              lcd.print(celestialObjectID);
              if (celestialObjectID.length() == 4) {
                lcd.clear();
                if (cdb.FindCelestialGotoObject(celestialObjectID, &targetObject))
                {
                  if (!targetObject.isAboveHorizon(getHour(), getMinute()))
                  {
                    lcd.print("error - object");
                    lcd.setCursor(0,1);
                    lcd.print("below horizon");
                    currentAction = INACTIVE;
                  }
                  else {
                    lcd.print(targetObject.name);
                    lcd.setCursor(0,1);
                    lcd.write(byte(0));
                    lcd.print(" or C to cancel");
                    currentAction = LOOKUP;
                  }
                }
                else
                {
                  lcd.print("error finding");
                  lcd.setCursor(0,1);
                  lcd.print("object " + celestialObjectID);
                  celestialObjectID = "";
                  currentAction = INACTIVE;
                } 
              }
            }
        break;
    }
    // slight delay to avoid unintentional duplicate key presses
    delay(100);
    irrecv.resume();
  }

  // state machine actions
  switch (currentAction) {
    case LOOKUP:
      // ready to look up object
      break;
    case READYTOGO:
      // waiting confirmation to slew
      break;
    case SLEWING:
      // slewing to object
      if (motors.completedSlew()) currentAction = INACTIVE;
      break;
    // manual move
    case key_up:
      motors.Move(10,0,calibrating); // alt az cal
      break;
    case key_down:
      motors.Move(-10,0,calibrating);
      break;
    case key_left:
      motors.Move(0,-10,calibrating);
      break;
    case key_right:
      motors.Move(0,10,calibrating);
      break;
    case TRACKING:
      // tracking
      targetPosition = targetObject.getCurrentAltAzPosition(getHour(), getMinute());
      motors.setTarget(targetPosition.alt, targetPosition.az);
      break;
    case SETHOUR:
    case SETMINUTE:
    case SETSECONDS:
          lcd.setCursor(0,0);
          lcd.print(getTimeString());
          break;
    default:
      break;
  }
}

