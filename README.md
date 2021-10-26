## ``` IntroductionToRobotics```
> Unibuc Robotics: Optional course in 2021-2022 academic year

---





- [x] Homework 1 (21-28 October 2021)
- **Technical Task**: Use a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control must be done
with digital electronics (aka you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected
to the led.
- **Components**: 1 RBG led , 3 potentiometers , resistors and wires (per logic)
- **Video showcase on youtube**: [link](https://youtu.be/mwl70TvYc9E)
- Picture of scene: ![image](https://user-images.githubusercontent.com/19687103/138956180-c529a7b8-f9c2-4b3b-b2d1-6f69311dc443.png)

- **Coding below**

```cpp
const int potRedPin = A0;
const int potGreenPin = A1;
const int potBluePin = A2;

const int maxBrightnessValue = 255;
const int maxAnalogValue = 1023;

const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

int potRedValue = 0;
int potGreenValue = 0;
int potBlueValue = 0;

int ledRedBrightness = 0;
int ledGreenBrightness = 0;
int ledBlueBrightness = 0;


void setup() {
  // The LEDS are output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // The potentiometers are input
  pinMode(potRedPin, INPUT);
  pinMode(potGreenPin, INPUT);
  pinMode(potBluePin, INPUT);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

}

void loop() {

  // read the input on analog pin 0:
  potRedValue = analogRead(potRedPin);
  // read the input on analog pin 1:
  potGreenValue = analogRead(potGreenPin);
  // read the input on analog pin 2:
  potBlueValue = analogRead(potBluePin);

  // Convert the analog reading (which goes from 0 - 1023) to LED brightness (which goes from 0 to 255)
  ledRedBrightness = map(potRedValue, 0, maxAnalogValue, 0, maxBrightnessValue);
  ledGreenBrightness = map(potGreenValue, 0, maxAnalogValue, 0, maxBrightnessValue);
  ledBlueBrightness = map(potBlueValue, 0, maxAnalogValue, 0, maxBrightnessValue);

  // The LEDs are connected on PWM output digital pins
  analogWrite(redPin, ledRedBrightness);
  analogWrite(greenPin, ledGreenBrightness);
  analogWrite(bluePin, ledBlueBrightness);

  // print brightness level on each RGB channel in Serial Monitor
  Serial.println(ledRedBrightness);
  Serial.println(ledGreenBrightness);
  Serial.println(ledBlueBrightness);
  delay(1);
}
```
