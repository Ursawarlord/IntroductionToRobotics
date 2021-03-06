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