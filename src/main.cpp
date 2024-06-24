#include "StepperControl.h"

// Define Stepper motor pins
#define StepperPin8 0
#define StepperPin9 1
#define StepperPin10 2
#define StepperPin11 3

#define ButtonPin 2

const int stepsPerRevolution = 2048; // Number of steps per revolution for the motor
const int motorSpeed = 15; // Speed in RPM

StepperControl myStepper(stepsPerRevolution, StepperPin8, StepperPin10, StepperPin9, StepperPin11); // Initialize the stepper motor

int main(void)
{
  // Initialize the stepper motor
  StepperControl::init();

  // Set the speed of the motor
  myStepper.setSpeed(motorSpeed);

  while(1)
  {

  }
}