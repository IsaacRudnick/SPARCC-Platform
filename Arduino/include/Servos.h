// Note: this file only defines "loose" servos. The shield servos are defined in the Adafruit_PWMServoDriver library.
#ifndef SERVOS_H
#define SERVOS_H

#include <Servo.h>

// YOU: Add as many loose servos as you want here, e.g.
// Servo servoName;

Servo looseServos[] = {
    // Include all loose servos here, e.g.
    // servoName,
};

// Number of loose servos (0 if none)
int looseServosCount = sizeof(looseServos) ? sizeof(looseServos) / sizeof(looseServos[0]) : 0;

// Call this in the setup() function to set up the servos
void setupServos()
{
    // YOU: Add configuration for more loose servos here in this format:
    // servoName.attach(pin);
    // e.g.,
    // servoName.attach(9);
}

#endif