#ifndef STEPPERS_H
#define STEPPERS_H

#include <AccelStepper.h>

AccelStepper testingStepper1 = AccelStepper(AccelStepper::DRIVER, 22, 23);
AccelStepper testingStepper2 = AccelStepper(AccelStepper::DRIVER, 24, 25);

AccelStepper steppers[] = {
    testingStepper1,
    testingStepper2,
};

// Call this in the setup() function to set up the steppers
void setupSteppers()
{
    steppers[0].setMaxSpeed(1250);
    steppers[0].setAcceleration(500);

    steppers[1].setMaxSpeed(1250);
    steppers[1].setAcceleration(500);
}

// Number of steppers (0 if none)
int steppersCount = sizeof(steppers) ? sizeof(steppers) / sizeof(steppers[0]) : 0;

#endif