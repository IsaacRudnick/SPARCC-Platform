from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize Servo that represents the one at index 0 in the looseServos array
MyServo = Servo(0,
                ServoConnectionType.LOOSE,
                ServoActuationType.POSITION,
                # This is what matters to the Arduino.
                (500, 2500),
                # The below range can be anything; it is only used for mapping degree ranges to pwm ranges
                # E.g., you could do (-90, 90)
                # OR EVEN (0, 1000) but that would not align with the actual degrees.
                (0, 180),
                ServoInverted.NORMAL)

# Initialize an interface for this arduino
Arduino = ArduinoInterface(PORT,
                           [MyServo],
                           [],
                           [])

print(f"Servo degrees are capped between {MyServo.actuation_range[0]} and {MyServo.actuation_range[1]}")
# Continuously ask user for servo degrees.
while True:
    degrees = int(input("Enter degrees for the servo. "))
    Arduino.set_servo(MyServo, degrees)
