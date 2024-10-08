from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize Sensor that we connected to pin 2.
# The 0 represents its index in the Sensors.h file, NOT ITS PIN(S)
ButtonSensor = Sensor(0)

# Initialize an interface for this arduino
Arduino = ArduinoInterface(PORT,
                           [],
                           [],
                           [ButtonSensor])

while True:
    print(Arduino.poll_sensor(ButtonSensor))
