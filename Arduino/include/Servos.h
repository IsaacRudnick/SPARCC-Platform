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

// Length of servos array is byte size of servos divided by byte size of first element. If no elements, length is 0.
int looseServosCount = sizeof(looseServos) ? sizeof(looseServos) / sizeof(looseServos[0]) : 0;