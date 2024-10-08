#include <Arduino.h> // Required for Arduino-specific functions like pinMode()

// Base Sensor class to handle any number of pins
class Sensor
{
protected:
    int *pins;    // Array to hold pins
    int pinCount; // Number of pins

public:
    Sensor(int pinArray[], int count)
    {
        pinCount = count;
        pins = new int[pinCount];
        for (int i = 0; i < pinCount; i++)
        {
            pins[i] = pinArray[i];
        }
    }

    // Default setup for sensors: Set all pins as INPUT (can be overridden)
    virtual void setup()
    {
        for (int i = 0; i < pinCount; i++)
        {
            pinMode(pins[i], INPUT);
        }
    }

    // Virtual read function to be implemented by each sensor
    virtual int read() = 0;
};

// Example: A temperature sensor using one analog pin
class TempSensor : public Sensor
{
public:
    TempSensor(int pin) : Sensor(&pin, 1) {}

    int read() override
    {
        return analogRead(pins[0]); // Read from the analog pin
    }
};

// Example: An ultrasonic sensor using two pins (trig and echo)
// Untested code, may need adjustments
class UltrasonicSensor : public Sensor
{
public:
    UltrasonicSensor(int trigPin, int echoPin) : Sensor(new int[2]{trigPin, echoPin}, 2) {}

    void setup() override
    {
        pinMode(pins[0], OUTPUT); // Trig pin
        pinMode(pins[1], INPUT);  // Echo pin
    }

    int read() override
    {
        // Ultrasonic sensor specific logic
        digitalWrite(pins[0], LOW);
        delayMicroseconds(2);
        digitalWrite(pins[0], HIGH);
        delayMicroseconds(10);
        digitalWrite(pins[0], LOW);

        long duration = pulseIn(pins[1], HIGH);
        int distance = duration * 0.034 / 2; // Convert time to distance
        return distance;
    }
};

class ButtonSensor : public Sensor
{
public:
    ButtonSensor(int pin) : Sensor(&pin, 1) {}

    void setup() override
    {
        pinMode(pins[0], INPUT_PULLUP); // Button with internal pull-up resistor
    }

    int read() override
    {
        return digitalRead(pins[0]);
    }
};

// Sensor array to store multiple sensors
Sensor *sensors[] = {
    new ButtonSensor(2),
};

const int sensorsCount = sizeof(sensors) / sizeof(sensors[0]);

// Setup all sensors
void setupSensors()
{
    for (int i = 0; i < sensorsCount; i++)
    {
        sensors[i]->setup();
    }
}
