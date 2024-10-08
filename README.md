# Serial Perception-Action Robotic Control and Communication Platform (SPARCC Platform)

This system was developed for use on Vassar College's HARPER project, a robotic platform for research and education. It is designed to abstract the communication between a Python program and an Arduino, allowing for easy control of PWM servos, stepper drivers, and sensors. The system is designed to be easily extensible, allowing for the addition of new components with minimal changes to the existing code. An example of it in use can be seen [here](https://www.github.com/IsaacRudnick/Whiteboard-Plotter).

# Overview

A Python class (ArduinoInterface) sends serial commands and queries to a connected Arduino/microcontroller (designed for a Mega2560, but any microcontroller works), collecting response data. When adding a new servo, stepper, or sensor, configuration is required on BOTH ends (local Python and compiled C++ Arduino Code). "Sensors" can refer to any input device or to software sensors (e.g., querying the arduino for the current time).

# Should I Use This For My Project?

TODO: Add this section, possibly with a flowchart

# Example Usage

This brief overview shows how the Arduino supports:

- mass 4.8-6V servo control with an Adafruit Shield
- custom-voltage ("loose") servos
- stepper motors
- analog and digital sensors

![Wiring Diagram](/media/example_assembly.png)

# Software Overview

There are three classes that the ArduinoInterface uses:

- [Servo.py](/Servo.py)

Represents shield and loose servos.

- [Stepper.py](/Stepper.py)

Represents a stepper motor, accounting for inversion.

- [Sensor.py](/Sensor.py)

Represents any sensor attached to the Arduino. For more complex sensors, it is recommended to either integrate their information higher up in the P&A loop or extend the Sensor class and define more robust functions.

The ArduinoInterface class must be initialized with all servos, steppers, and sensors, along with being provided the port the Arduino is connected to (more on that later).

On the Arduino side, all sensors, servos, and steppers must be represented in their respective header file (see [Servos.h](/Arduino/include/Servos.h) as an example). To send code to the Arduino, this project is configured to use [PlatformIO](https://platformio.org/), a VSCode Extension.

Commands (and groups thereof) are abstracted; you never need to know what they look like to use this architecture. However, for troubleshooting, it can be helpful. Below, an example command is shown. More information is provided in the [main.cpp](/Arduino/src/main.cpp) file, which is commented with multiple examples.
"s2=1000;t1=200;i0?;" sets shield servo 2 to 1000, moves stepper 1 to 200, and queries sensor 0 (responding with the value).

For more information on setting up these components, see the [example walkthrough](/docs/ExampleSetupWalkthrough.md)

# Hardware Overview

The Arduino should be connected over a USB-3+ connection. If you're not sure what this means, it's ok to ignore. USB-3 (usually indicated by being colored inside the port) is faster. It's ok to use a USB dock / dongle adapting from USB-C, which is generally very fast.

The currently-configured PlatformIO setup is for an Arduino Mega 2560. This can be changed, but it is recommended to use this Arduino because it has many GPIO pins and a relatively fast processor.

For more information on setting up these components, see the [example walkthrough](/docs/ExampleSetupWalkthrough.md)

# Troubleshooting

See the [troubleshooting guide](/docs/Troubleshooting.md) for common issues and solutions.