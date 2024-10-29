from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# The 0 represents its index in the Steppers.h file, NOT ITS PINS
MyStepper = Stepper(0, StepperDirection.NORMAL)

# Initialize an interface for this arduino
Arduino = ArduinoInterface(PORT,
                           [],
                           [MyStepper],
                           [])

while True:
    target = float(input("Enter revolutions for the stepper. "))
    target = int(target * 200)  # 200 steps per revolution
    Arduino.set_stepper(MyStepper, target)
