// Note: this file only defines "loose" servos. The shield servos are defined in the Adafruit_PWMServoDriver library.
#ifndef SERVOS_H
#define SERVOS_H

#include <Servo.h>

Servo myServo1;
Servo myServo2;
// YOU: Add as many loose servos as you want here

Servo looseServos[] = {
    myServo1,
    myServo2,
};

// Number of loose servos (0 if none)
int looseServosCount = sizeof(looseServos) ? sizeof(looseServos) / sizeof(looseServos[0]) : 0;

// Call this in the setup() function to set up the servos
void setupServos()
{
    looseServos[0].attach(4);
    looseServos[1].attach(5);
    // YOU: Add configuration for more loose servos here
}

#endif