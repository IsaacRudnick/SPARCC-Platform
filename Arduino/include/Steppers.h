#ifndef STEPPERS_H
#define STEPPERS_H

#include <AccelStepper.h>

// First pin is for step, second pin is for direction
AccelStepper myStepper1 = AccelStepper(AccelStepper::DRIVER, 23, 22);
// YOU: Add as many steppers as you want here

AccelStepper steppers[] = {
    myStepper1,
};

// Number of steppers (0 if none)
int steppersCount = sizeof(steppers) ? sizeof(steppers) / sizeof(steppers[0]) : 0;

// Call this in the setup() function to set up the steppers
void setupSteppers()
{
    // Be careful: this will not work if you replace steppers[n] with the stepper object directly
    steppers[0].setMaxSpeed(1250);
    steppers[0].setAcceleration(500);
    // YOU: Add configuration for more steppers here
}

#endif