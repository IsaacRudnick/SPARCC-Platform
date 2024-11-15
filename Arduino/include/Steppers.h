#ifndef STEPPERS_H
#define STEPPERS_H

#include <AccelStepper.h>

// Stepper initialization will follow this format:
// AccelStepper stepperName = AccelStepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
// YOU: Add as many steppers as you want here

AccelStepper steppers[] = {
    // Include all steppers here, e.g.
    // stepperName,
};

// Number of steppers (0 if none)
int steppersCount = sizeof(steppers) ? sizeof(steppers) / sizeof(steppers[0]) : 0;

// Call this in the setup() function to set up the steppers
void setupSteppers()
{
    // Initialize steppers here.
    // Max speed the Arduino can handle is ~4000 (revs/second).
    // Max accel is project-based and code can handle any accel.
    // For each stepper, set max speed and acceleration like this:
    // steppers[n].setMaxSpeed(speed);
    // steppers[n].setAcceleration(acceleration);
    // Be careful: this will not work if you replace steppers[n] with the stepper object directly
    // YOU: Add configuration for more steppers here
}

#endif