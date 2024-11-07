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

Example configuration of these components is shown in the below diagram. There are "loose" and "shield" servos, a stepper driver, and a button sensor.
![Wiring Diagram](/docs/media/example_assembly.png)

## PlatformIO, VSCode, and Environment Setup

Install the PlatformIO Extension on VSCode. On the left side of your screen (the "Activity Bar"), you should see a small alien-head icon.

Open two VSCode Windows, one for the root folder (we will call this the "main window") and the other for the `Arduino` folder (we will call this the "Arduino window"). It is important you do this properly. To ensure VSCode is open to a folder, you should select File -> Open Folder -> [Folder].

The reason we use two windows is so PlatformIO in the Arduino window recognizes the "project" and its configuration file.

To ensure this is set up correctly:

1. Open the Arduino window
1. Click the PlatformIO (alien) icon

You should see a variety of options under "Project Tasks" such as "Upload"

If you do not see these options, you may be in the wrong window, may not have the PlatformIO extension installed, or something else. Make sure you followed the above steps correctly.

Next, we must make sure we have all of the Python requirements installed. You can do this by running one of the below commands in the main window's terminal.

```bash
pip install -r requirements.txt
# or, if that doesn't work,
python -m pip install -r requirements.txt
# or, if that doesn't work,
python3 -m pip install -r requirements.txt
```

You may also use a venv or conda environment if you prefer, but this guide will not cover that.

## Connecting to an Arduino

Connect to an Arduino Mega 2560 using a USB-B cable. It is ok to use a USB dock / dongle for this connection. You can use another Arduino, but you will need to change the board in the PlatformIO.ini configuration file. This guide assumes you are using a Mega 2560.

When the cable is connected, one or more lights on the Arduino should turn on, indicating the powered-on status.

Open Platformio. Under "Quick Access" -> "PIO Home" -> "Devices." One of the devices should look like:

- "USB Serial Device (COM9)" on Windows
- "USB Serial Device (/dev/ttyUSB1)" on Linux/Mac
- "/dev/cu.usbmodem142401" on Mac if a USB hub is being used

This will take some reasonable interpretation. If you're not sure, remove the Arduino, refresh the list, and see which device disappears. Then, plug the Arduino back in and see which device reappears. This is the Arduino.

Take note of this port. We will use/assume "COM9" for this tutorial, but it will vary.

## Adding Motors and Sensors

This section will walk you through adding servos, steppers, and sensors to the codebase. The Python code will then interact with these components. Feel free to put your python code in the `main.py` file in the main window.

> You can name the file whatever you want, but it is important that the file is in the main window. If you put it in the Arduino window, it will not be able to access the ArduinoInterface module.

### Servo Motors

#### Shield Servos

First, we will set up and use shield servos. To do so, we must connect the [specific Adafruit shield](https://www.adafruit.com/product/1411) to the Arduino. A red light on the shield should turn on.

Next, connect 4.8-6V to the screwdown terminal of the shield. Once the power is on, a green light should turn on next to the red light. This power supply is used by the shield for all attached servos, and is separate from the 5V Arduino power supply.

Connect a servo to the shield. For this tutorial, we will be using an "SG90 9G Micro Servo" which has a 500-2500 microsecond pulse range for its PWM duty signal.

> You don't have to know what this means, but it is important to know that different servos have different pulse ranges. If your servo is different, you will need to adjust the pulse range in the Python code. You can find this information in the servo's datasheet, which you can find by googling "{Servo Model} datasheet".

Connect the servo to the shield. The three servo wires represent
| Color | Purpose |
|---|---|
| Yellow / Orange | PWM (the data) |
| Red | V+ |
| Black / Brown | GND |

The shield is labeled to show you how to align the wires

For this tutorial, we will connect to pin set 7 (labeled on the shield).

In the Arduino Window, open PlatformIO and select "Upload and Monitor." If there are errors, try unplugging the Arduino, restarting VSCode, and plugging it back in.

Once the code is uploaded, the terminal should show that the Arduino is ready by printing "Ready."

At that point, **press Ctrl+C to close the monitoring process**. This frees up the serial connection for our Python code.

In the main window, create a python file (or use main.py) and write this code to initialize the servo, replacing the port with your port from earlier.

```python
from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize servo that we connected to pin set 7
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
# Continuously ask user for servo degrees. You could do whatever you want here, though.
while True:
    degrees = int(input("Enter degrees for the servo. "))
    Arduino.set_servo(MyServo, degrees)
```

Run the Python code.

Assuming there are no errors, you should be prompted for a degree value for the servo to go to. Try some values out. Notice that entering a value outside the degree range does not harm the servo; the value is capped by the `Servo` class.

Get a good feel for using the servo class. Try setting up a second servo, moving this one, or even adjusting the degree range (e.g. from `(0, 180)` to `(-90, 90)`)

#### "Loose" (Non-Shield) Servos

Now, we will set up a "loose" servo. For servos that you wish not to connect to the shield (e.g., those taking a different voltage), you can connect their V+/GND wires directly to your other power source, and their signal wires directly to the Arduino. For simplicity, we will use a regular servo that _could_ connect to the shield, but we will connect it directly to the Arduino.

First, connect the servo's V+ and GND wires to a power source.

> you can use the Arduino's 5V and GND pins if you're using the SG90 9G Micro Servo, but if you had an actual high-voltage or many servos, you'd need a dedicated power source.

Then, connect the PWM/signal wire to the Arduino. You can use most digital pins, not just the ones labeled with a `~` or `PWM` next to them.

For simplicity, we will use pin 7 for this servo, but you can use any digital pin except those labeled for communication (pins 0-1 & 14-21 on a mega) since these can cause issues.

Since this servo is a "loose" servo, we will need to set it up on the Arduino side, too. In the Arduino code, you will need to configure the servo. This is done in the [/Arduino/include/Servos.h](/Arduino/include/Servos.h) file. This is what that file might look like. To add more servos, just look for comments that start with `YOU:` and follow the instructions.

```cpp
// Note: this file only defines "loose" servos. The shield servos are defined in the Adafruit_PWMServoDriver library.
#ifndef SERVOS_H
#define SERVOS_H

#include <Servo.h>

Servo myServo1;
// YOU: Add as many loose servos as you want here

Servo looseServos[] = {
    myServo1,
};

// Number of loose servos (0 if none)
int looseServosCount = sizeof(looseServos) ? sizeof(looseServos) / sizeof(looseServos[0]) : 0;

// Call this in the setup() function to set up the servos
void setupServos()
{
    looseServos[0].attach(7);
    // YOU: Add configuration for more loose servos here
}

#endif
```

Follow the same steps as before to upload the code to the Arduino, ensuring that you close the monitoring process with Ctrl+C when the code is uploaded and ready.

Now, we can interact with the servo in Python (in the main window). When we used a shield servo, we referenced it by its pin number. For loose servos, the Python code will instead reference them by their index in the `looseServos` array in the Arduino file (0 in this example). Here's what your code might look like (given the above configuration in the Arduino code):

```python
from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize servo that represents the one at index 0 in the looseServos array
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
# Continuously ask user for servo degrees. You could do whatever you want here, though.
while True:
    degrees = int(input("Enter degrees for the servo. "))
    Arduino.set_servo(MyServo, degrees)
```

Make sure to replace the port with your port from earlier.

If you are running many servos from 4.8-6V, it is recommended to just use the shield. However, for projects requiring only a few servos, or servos with different voltage requirements, loose servos are a good option. You can initialize loose and shield servos together, allowing for a mix. You can also add more servos, change the degree range, or invert the servo in the Arduino code.

### Stepper Motors

Stepper motors are more complicated than these servos, mainly because they require more assembly. The stepping (constant, alternating pulsing of current) of the stepper is abstracted, you don't need to deal with it. However, you do need to wire a stepper motor driver.

Here's what it looks like (adapted from the base wiring diagram)
![Stepper Wiring Diagram](/docs/media/stepper_wiring.png)

> Please note that for some steppers, the order of the wires (often called A+, A-, B+, B-) differs. There is **not** a standard for this, so you may need to try a few things. If you are unsure, try to google "{Stepper Model} datasheet" to find a datasheet, which may include a diagram telling you which wire is which. Then, you can plug the correct wires into the appropriate A/B +/- terminals.

In the Arduino code (in the Arduino window), you will need to configure the stepper. This is done in the [/Arduino/include/Steppers.h](/Arduino/include/Steppers.h) file. This is what your stepper configuration might look like. To add more steppers, just look for comments that start with `YOU:` and follow the instructions.

```cpp
#ifndef STEPPERS_H
#define STEPPERS_H

#include <AccelStepper.h>

// First pin is for step, second pin is for direction
AccelStepper myStepper1 = AccelStepper(AccelStepper::DRIVER, 23, 22);
// YOU: Add as many steppers as you want here

AccelStepper steppers[] = {
    myStepper1,
};

// Number of steppers (0 if none)
int steppersCount = sizeof(steppers) ? sizeof(steppers) / sizeof(steppers[0]) : 0;

// Call this in the setup() function to set up the steppers
void setupSteppers()
{
    // Be careful: this will not work if you replace steppers[n] with the stepper object directly
    steppers[0].setMaxSpeed(1250);
    steppers[0].setAcceleration(500);
    // YOU: Add configuration for more steppers here
}

#endif
```

Follow the same steps as before to upload the code to the Arduino, ensuring that you close the monitoring process with Ctrl+C when the code is uploaded and ready.

Now, we can interact with the stepper in Python. Here is an example of how you might do that:

```python
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

# Continuosly ask user for stepper rotation/position. You could do whatever you want here, though.
while True:
    target = float(input("Enter revolutions for the stepper. "))
    target = int(target * 200)  # 200 steps per revolution
    Arduino.set_stepper(MyStepper, target)
```

Make sure to replace the port with your port from earlier.

Make sure your stepper can turn clockwise and counterclockwise. If it can't, look in the [troubleshooting document](docs/Troubleshooting.md) for help.

Once the above works properly, you can try adding more steppers, changing the direction, or changing the number of steps per revolution. You can also try changing the speed and acceleration in the Arduino code. Finally, try making your own program in Python, such as making the stepper into a clock second hand.

### Sensors (and Software Sensors)

This tutorial will walk you through adding a simple digital button sensor, but you will see in the tutorial how adding other sensor types may be accomplished.

These can even include software sensors, such as a "sensor" that reads the current time or reports information about a stepper motor.

The below diagram shows a closeup of the wiring you must implement for a pushbutton which uses a V+/HIGH, GND/LOW, and signal wire. The signal wire will read as LOW (0) if the button is pressed and HIGH (1) if it is not.

The wiring of any sensor will depend on its specific requirements. The code will allow for some flexibility, e.g., in which pin(s) you use.

![Wiring Diagram](/docs/media/sensor_wiring.png)

The advantage of this system is in the codebase. In [/Arduino/include/Sensors.h](/Arduino/include/Sensors.h), you will configure the sensor.

The code for some sensors is already written. For example, the `ButtonSensor` class is already written. All you need to do to is add the sensor to the array of sensors near the bottom of the file. The `ButtonSensor` class takes in one parameter, the pin number of the sensor. In the figure above, that's pin 2, but you can use another pin if you want. Take caution using pins labeled for communication (pins 0-1 & 14-21 on a mega). They are sometimes reserved by the Arduino for serial communication, and may interfere with the ArduinoInterface.

This is what your sensor configuration might look like, at the bottom of the `Sensors.h` file:

```cpp
// Sensor array to store multiple sensors
Sensor *sensors[] = {
    // Initialize a ButtonSensor on pin 2
    new ButtonSensor(2),
};
```

Follow the same steps as before to upload the code to the Arduino, ensuring that you close the monitoring process with Ctrl+C when the code is uploaded and ready.

In the Python code (in the main window), we need to add a sensor to the ArduinoInterface. This is done by adding a `Sensor` object to the list of sensors. The `Sensor` object takes in the index of the sensor in the Arduino code's `sensors` array. In this example, that's 0, since the button sensor is at index 0 in the array. Here's what your Python code might look like:

```python
from ArduinoInterface import *

# Declare port that Arduino is connected to
PORT: str = "COM9"

# Initialize sensor that we connected to pin 2.
# The 0 represents its index in the Sensors.h file, NOT ITS PIN
MyButtonSensor = Sensor(0)

# Initialize an interface for this arduino
Arduino = ArduinoInterface(PORT,
                          [],
                          [],
                          [MyButtonSensor])

# Continuously poll the sensor. You could do whatever you want here, though.
while True:
    print(Arduino.poll_sensor(MyButtonSensor))
```

Make sure to replace the port with your port from earlier.

Notice that, while relatively fast to the human eye, the polling rate of this system is ~10 queries per second. Thus, you may see a delay between pressing the button and the Python code printing the change, but that delay should never be more than ~100ms.

Try combining the code you wrote in the Servo and Sensor sections. For example, you could have the servo move to a certain position when the button is pressed and move back when it is released.

While `ArduinoInterface.poll_sensor` does returns the polled value, it is also stored in the `Sensor` instance. Thus, you can also access the value by checking the `Sensor` instance's `value` attribute. In practice, this means that:

```python
print(Arduino.poll_sensor(MyButtonSensor))
```

is the same as

```python
Arduino.poll_sensor(MyButtonSensor)
print(MyButtonSensor.value)
```

and `Sensor` instances also record when they last stored a value, so you can see how recent the value is by checking the timestamp attribute.

```python
Arduino.poll_sensor(MyButtonSensor)
print(MyButtonSensor.value)
print(f"Polled at {MyButtonSensor.timestamp}")
```

> Note, this is when the `Sensor` instance received the value, **not** when the Arduino polled the sensor. There may be a delay between the two, but it should never be more than ~100ms.

## Conclusion

This tutorial has walked you through setting up PlatformIO, connecting to an Arduino, adding servos, steppers, and sensors, and using/polling these items through Python code. You should now have a good understanding of how to use the ArduinoInterface to interact with these components. You can now try adding more components, changing the configuration of the components, or making your own programs in Python. If you have any issues, check the [troubleshooting document](docs/Troubleshooting.md) for help.
