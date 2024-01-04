#ifndef motors_h
#define motors_h

#include <AccelStepper.h>

class stepperMotors 
{
  public:

  void init()
  {
    azfullcirclesteps = 200 * 32;
    altfullcirclesteps = 200 * 32;
    moveamount = 0;
    maxmoveamount = 500;

    // position zero is alt = 0, az = 0 (so tube vertical, az base set to north
    // - this corresponds to storage position, so easier set up)
    // user starts the scope like this (using compasses, spirit levels etc), 
    // then does a goto to a known object. Once there, turn on calibrate mode,
    // manual move to center the object, and turn off calibrate mode (calibrate
    // mode simply doesn't update the stored current position during a manual move)
    AzStepper = new AccelStepper(AccelStepper::DRIVER, 14,15); // step A0, dir A1 
    AzStepper->setMaxSpeed(500.0);
    AzStepper->setAcceleration(100.0);
    AzStepper->setCurrentPosition(0);

    AltStepper = new AccelStepper(AccelStepper::DRIVER, 16,17);    // step A2, dir A3
    AltStepper->setMaxSpeed(500.0);
    AltStepper->setAcceleration(100.0);
    AltStepper->setCurrentPosition(getStepsFordegrees(true, 90));

  }

  void update()
  {
    AzStepper->run();
    AltStepper->run();
  }

  int getStepsFordegrees(bool alt, double angle)
  {
    return (int)(angle * ((alt)? altfullcirclesteps : azfullcirclesteps) / 360.0 );
  }

  void Move(int altsteps, int azsteps, bool continuouspress, bool calibrate)
  {
    moveamount = (continuouspress) ? moveamount * 2 : 1;
    if (moveamount > maxmoveamount) moveamount = maxmoveamount;
    AltStepper->moveTo(AltStepper->currentPosition() + altsteps*moveamount);
    AzStepper->moveTo(AzStepper->currentPosition() + azsteps*moveamount);
  }

  void setTarget(double alt, double az)
  {
    AltStepper->moveTo(getStepsFordegrees(true, alt));
    AzStepper->moveTo(getStepsFordegrees(false, az));
  }

  bool completedSlew()
  {
    return (AltStepper->currentPosition() == AltStepper->targetPosition() &&
            AzStepper->currentPosition() == AzStepper->targetPosition());
  }

private:
  // steps to move the telescope a full circle (not full circle of motor)

  AccelStepper* AzStepper; 
  AccelStepper* AltStepper;
  int moveamount;
  int maxmoveamount;
  int azfullcirclesteps;
  int altfullcirclesteps;
};

#endif // motors_h