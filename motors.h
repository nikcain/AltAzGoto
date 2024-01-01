#ifndef motors_h
#define motors_h

#include <AccelStepper.h>

class stepperMotors 
{
  public:
  AccelStepper* AzStepper; 
  AccelStepper* AltStepper;

  void init()
  {
    // position zero is alt = 0, az = 0 (so tube horizontal, pointing north)
    // user starts the scope like this (using compasses, spirit levels etc), 
    // then does a goto to a known object. Once there, turn on calibrate mode,
    // manual move to center the object, and turn off calibrate mode (calibrate
    // mode simply doesn't update the stored current position during a manual move)
    AzStepper = new AccelStepper(AccelStepper::DRIVER, 14,15); // A0, A1 
    AzStepper->setMaxSpeed(200.0);
    AzStepper->setAcceleration(100.0);
    AzStepper->setCurrentPosition(0);

    AltStepper = new AccelStepper(AccelStepper::DRIVER, 16,17);    // A2, A3
    AltStepper->setMaxSpeed(300.0);
    AltStepper->setAcceleration(100.0);
    AltStepper->setCurrentPosition(0);
  }

  void update()
  {
    AzStepper->run();
    AltStepper->run();
  }

  void Move(int altsteps, int azsteps, bool calibrate)
  {

  }

  void setTarget(double alt, double az)
  {

  }

  bool completedSlew()
  {
    return (AltStepper->currentPosition() == AltStepper->targetPosition() &&
            AzStepper->currentPosition() == AzStepper->targetPosition());
  }

};

#endif // motors_h