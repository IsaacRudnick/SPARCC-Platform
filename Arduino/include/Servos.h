// Note: this file only defines "loose" servos. The shield servos are defined in the Adafruit_PWMServoDriver library.
#ifndef SERVOS_H
#define SERVOS_H

#include <Servo.h>

Servo testingServo1;
Servo testingServo2;

Servo looseServos[] = {
    testingServo1,
    testingServo2,
};

// Call this in the setup() function to set up the servos
void setupServos()
{
    looseServos[0].attach(22);
    looseServos[1].attach(23);
}

// Number of loose servos (0 if none)
int looseServosCount = sizeof(looseServos) ? sizeof(looseServos) / sizeof(looseServos[0]) : 0;

#endif