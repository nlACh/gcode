//required for test functions...
//helps me clean out the mess
//temporary functions waiting to be staged
//will live here
//ACCEL::
#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper sX(AccelStepper::FULL4WIRE, 2, 4, 3, 5);
AccelStepper sY(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

MultiStepper steppers;
void setup()
{
  Serial.begin(9600);

  sX.setMaxSpeed(100);
  sY.setMaxSpeed(100);

  steppers.addStepper(sX);
  steppers.addStepper(sY);
}

void loop()
{
  long pos[2];
  pos[0] = 0;
  pos[1] = 2048;
  steppers.moveTo(pos);
  steppers.runSpeedToPosition();
  delay(10000);
}

