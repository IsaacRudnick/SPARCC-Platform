# Servo, Stepper, and Sensor Setup

This guide will walk you through

- setting up PlatformIO
- connecting to an Arduino
- adding
  - several servos
  - a stepper motor
  - a sensor
- using / polling these items through Python code

Tutorial Prerequisites:

- Python (Tested on 3.12)
- VSCode
- Arduino w/ USB Cable
- PWM Servo
- Stepper Motor and DM320T Stepper Motor Driver
- Digital Button Sensor

## PlatformIO and VSCode Setup

Install the PlatformIO Extension on VSCode. On the left side of your screen (the "Activity Bar"), you should see a small alien-head icon.

Open two VSCode Windows, one for the `Movement` folder and the other for the `Movement/Arduino` folder. It is important you do this properly. To ensure VSCode is open to a folder, you should select File -> Open Folder -> [Folder]

The reason we use two windows is so PlatformIO in the second window recognizes the "project" and its configurations.

To ensure this is set up correctly:

1. Open the "Arduino" window
1. Click the PlatformIO (alien) icon
1. You should see a variety of options under "Project Tasks" such as "Upload"

Next, we must make sure we have all of the Python requirements installed. You can do this by running one of the below commands.

```bash
pip install -r requirements.txt
# or, if that doesn't work,
python -m pip install -r requirements.txt
# or, if that doesn't work,
python3 -m pip install -r requirements.txt
```

You may also use a venv or conda environment if you prefer, but this guide will not cover that.

## Connecting to an Arduino

Connect to an Arduino Mega 2560 using a USB-B cable. It is ok to use a USB dock / dongle for this connection.

When the cable is connected, one or more lights on the Arduino should turn on, indicating the powered-on status.

Open Platformio. Under "Quick Access" -> "PIO Home" -> "Devices." One of the devices should say something like "USB Serial Device (COM9)" on Windows or "USB Serial Device (/dev/ttyUSB1)" on Linux/Mac.

Take note of this port. We will use/assume "COM9" for this tutorial, but it will vary.

## Adding Motors and Sensors

### Servo Setup

First, we will set up and use shield servos. To do so, we must connect the [specific Adafruit shield](https://www.adafruit.com/product/1411) to the Arduino. A red light on the shield should turn on.

Next, connect 4.8-6V to the screwdown terminal of the shield. Once the power is on, a green light should turn on next to the red light. This power supply is used by the shield for all attached servos, and is separate from the 5V Arduino power supply.

Connect a Servo to the shield. For this tutorial, we will be using an "SG90 9G Micro Servo" which has a 500-2500 microsecond pulse range

Connect the servo to the shield. The three servo wires represent
| Color | Purpose |
|---|---|
| Yellow / Orange | PWM |
| Red | V+ |
| Black / Brown | GND |

The shield is labeled to show you how to align the wires.

For this tutorial, we will connect to pin set 7 (labeled on the shield).

Create a python file (or use main.py) and write this code to initialize the servo

```python
from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize Servo that we connected to pin set 7
MyServo = Servo(7,
                ServoConnectionType.SHIELD,
                ServoActuationType.POSITION,
                # This is what matters to the Arduino.
                (500, 2500),
                # This only matters to the "user"
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
```

In the Arduino Window, open PlatformIO and select "Upload and Monitor." If there are errors, try unplugging the Arduino, restarting VSCode, and plugging it back in.
Once the code is uploaded, the terminal should display "Initializing..."
At that point, press Ctrl+C to close the monitoring process. This frees up the serial connection for our Python code
Go to the Python window and run the above code.

Assuming there are no errors, you should be prompted for a degree value for the servo to go to. Try some values out. Notice that entering a value outside the degree range does not harm the servo; the value is capped by the Servo class.

Get a good feel for using the servo class. Try setting up a second servo, moving this one, or even adjusting the degree range (e.g. from `(0, 180)` to `(-90, 90)`)

### Stepper Setup

Stepper motors are more complicated than these servos, mainly because they require more assembly. The stepping (constant, alternating pulsing of current) of the stepper is abstracted, you don't need to deal with it. However, you do need to wire a stepper motor driver.

Here's what it looks like (adapted from the base wiring diagram)
![Stepper Wiring Diagram](/media/stepper_wiring.png)

> Please note that for some steppers, the order of the wires (often called A+, A-, B+, B-) differs. There is **not** a standard for this, so you may need to try a few things. If you are unsure, try to google "{Stepper Model} filetype:pdf" to find a datasheet, which may include a diagram telling you which wire is which. Then, you can plug the correct wires into the appropriate A/B +/- terminals.

TODO: This code and setup is not tested.

```python
from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize Stepper that we connected to pins TODO: ???
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
```

### Sensor Setup

This tutorial will walk you through adding a simple digital button sensor, but you will see in the tutorial how adding other sensor types may be accomplished.

The below diagram shows a closeup of the wiring you must implement for a pushbutton which uses a V+/HIGH, GND/LOW, and signal wire. The signal wire will read as LOW (0) if the button is pressed and HIGH (1) if it is not.

The wiring of any sensor will depend on its specific requirements. The code will allow for some flexibility, e.g., in which pin(s) you use.

![Wiring Diagram](/media/servo_and_sensor_wiring.png)

THe advantage of this system is in the codebase. In
[./Arduino/include/Sensors.h](/Arduino/include/Sensors.h), you will configure the sensor.

The code for some sensors is already written. For example, the `ButtonSensor` class is already written. All you need to do to is add the sensor to the array of sensors near the bottom of the file. The `ButtonSensor` class takes in one parameter, the pin number of the sensor. In the figure above, that's pin 2, but you can use another pin if you want. DO NOT use pins 1 or 2. They are reserved by the Arduino for serial communication, and will interfere with the ArduinoInterface.

```c
// Sensor array to store multiple sensors
Sensor *sensors[] = {
    // Initialize a ButtonSensor on pin 2
    new ButtonSensor(2),
};
```

In the Python code, we need to add a sensor to the ArduinoInterface. This is done by adding a `Sensor` object to the list of sensors. The `Sensor` object takes in the pin number of the sensor.

```python
from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize Sensor that we connected to pin 2.
# The 0 represents its index in the Sensors.h file, NOT ITS PIN
ButtonSensor = Sensor(0)

# Initialize an interface for this arduino
Arduino = ArduinoInterface(PORT,
                          [],
                          [],
                          [ButtonSensor])

while True:
    print(Arduino.poll_sensor(ButtonSensor))
```

Notice that, while relatively fast to the human eye, the polling rate of this system is ~10 queries per second. Thus, you may see a delay between pressing the button and the Python code printing the change, but that delay should never be more than ~100ms.

Try combining the code you wrote in the Servo and Sensor sections. For example, you could have the servo move to a certain degree when the button is pressed and move back when it is released.

While ArduinoInterface.poll_sensor does returns the polled value, it is also stored in the Sensor instance. Thus, you can also access the value by checking the Sensor instance's value attribute. Thus,

```python
print(Arduino.poll_sensor(ButtonSensor))

# is the same as

Arduino.poll_sensor(ButtonSensor)
print(ButtonSensor.value)
```

and Sensor instances also record when they last stored a value, so you can see how recent the value is by checking the timestamp attribute.

```python
Arduino.poll_sensor(ButtonSensor)
print(ButtonSensor.value)
print(f"Polled at {ButtonSensor.timestamp}")
```

> Note, this is when the Sensor instance received the value, **not** when the Arduino polled the sensor. There may be a delay between the two, but it should never be more than ~100ms.