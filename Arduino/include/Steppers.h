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

// Length of steppers array is byte size of steppers divided by byte size of first element. If no elements, length is 0.
int steppersCount = sizeof(steppers) ? sizeof(steppers) / sizeof(steppers[0]) : 0;
