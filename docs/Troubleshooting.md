# Troubleshooting

## Serial Port Usage / Holding

An important thing to note is that multiple processes may not use the same serial connection. For example, if you "Upload and Monitor" with Platformio, it will compile and upload the code and then act as a serial monitor. Because of the latter action, you will not be able to run any Python code that makes use of the ArduinoInterface class, since that class also uses a serial connection to communicate with the Arduino.

## "Fried" Components

Running too many volts or amps through certain components, as well as incompatible instructions, may break them permanently. A trivial example is servo motors; if given a PWM range too far beyond their upper / lower bound, they may break forever. The Python classes clamp values to the provided upper/lower bound, but are only useful if the provided bounds are correct. Generally, test things with cheaper / simpler components before using more complicated ones.

## Serial Disconnects/Interrupts

Make sure the Arduino Communication pins (on a Mega, pins 0-1 & 14-21) are not being used, unless you are doing so intentionally. This includes digital pins 0 and 1, which are used for serial communication. If you are using these pins for something else, it may cause an interruption in communication.

## Code Not Uploading Properly

If you are getting an error in uploading the code, try

1. restarting VSCode
1. Powering off the Arduino, waiting 10 seconds, and powering it back on
1. restarting the Computer
1. Getting a new USB-B cable
1. unplugging the Arduino and plugging it back in
1. Pressing 'reset' on the Arduino right before uploading the code
1. Running 'Full Clean' in PlatformIO, under 'Project Tasks'
1. Audibly declaring "I'll just get a new Arduino" (this may scare the Arduino into working)
1. Getting a replacement Arduino and trying to upload to that one.

## Stepper Motor Not Moving or Only Moving One Direction

Make sure that the OPTO terminal is receiving 5V and that the DIR pin/wire is outputting correctly. One way to do this is to move the DIR wire so that instead of connecting to the DM320T stepper driver, it powers an LED circuit. Then, when the code sets the direction of the stepper, the LED should reflect the direction (on for one direction and off for another). If it is always on or off, you should check the connections and ensure you've followed the setup instructions (see the [example setup walkthrough](/docs/ExampleSetupWalkthrough.md)).

## Servo Not Moving

It is possible the servo is broken. Try using a different servo to see if the problem persists. If it still doesn't work, check the wiring and ensure the servo is receiving power. If it still doesn't work, check the code and ensure the servo is being sent the correct PWM signal. This involves cross-referencing the Python upper/lower PWM bounds with the servo datasheet. Google "{servo model} datasheet" to find the datasheet (usually a pdf) which usually contains the PWM range. If there is no range, most servos are safe to use with a 1000-2000 range, but this is not guaranteed. DO NOT GUESS THE RANGE FOR A SERVO YOU ARE NOT WILLING TO POSSIBLY BREAK!

## Sensor Not Responding

Change your sensor read function to return a constant value (e.g., 2) and see if quierying the sensor works now. If it does, this implies your read function has an error. In this case, try to test your sensor read function in a dedicated project that [tries reading the sensor](https://stackoverflow.com/help/minimal-reproducible-example).

If the constant value does not work, your sensor is likely not initialized properly. Check the [example walkthrough](/docs/ExampleSetupWalkthrough.md) for how to initialize a sensor. Make sure you are matching the style used there.
