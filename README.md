## ``` IntroductionToRobotics```
> Unibuc Robotics: Optional course in 2021-2022 academic year

---

<details>
  
  <summary> Homework 1 (21-28 October 2021) </summary>
  
 #### RGB led and Potentiometers
+ **Technical Task**: Use a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control must be done
with digital electronics (aka you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected
to the led.
  
|**Components**:|1 RBG led| 3 potentiometers| resistors|and wires (per logic)|
|---|---|---|---|---|  
  
|**Video showcase on youtube**|[link](https://youtu.be/mwl70TvYc9E)|
|---|---|
  
+ Picture of scene: ![image](https://user-images.githubusercontent.com/19687103/138956180-c529a7b8-f9c2-4b3b-b2d1-6f69311dc443.png)

<details open>
  
  <summary> Show/Hide The Code</summary>

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

</details>
  
  </details>

---








<details>
  
  <summary> Homework 2 (27th October-3rd November 2021) </summary>
  
 #### Crosswalk simulator: traffic lights for people and cars
+ **Technical Task**:Building the traffic lights for a crosswalk. You will use 2 LEDs to represent the traffic lights for people (red and green)
and 3 LEDs to represent the traffic lights for cars (red, yellow and green). See the states it needs to go through. If anything is not clear, ask. Also, see the uploaded video (the intervals are different, but the states flow is the same). It’s a traffic lights system for people and cars - don’t overthink it.
  
|**Components**:|5 LEDs|1 button|1 buzzer|resistors|and wires (per logic)|
|---|---|---|---|---|---|
  
|**Video showcase on youtube**|[link](https://youtu.be/X81VE7nk7eY)|
|---|---|
  
+ Picture of scene: ![image](https://user-images.githubusercontent.com/19687103/140197526-f179777f-70d4-47e8-b831-541e97e5a1f0.jpeg)

<details open>
  
  <summary> Show/Hide The Code</summary>

```cpp
const int buttonPin = 2;
bool buttonState = LOW;

bool reading = LOW;
bool prevReading = LOW;

const int driverRedPin = 9;
bool driverRedState = LOW;

const int driverYellowPin = 8;
bool driverYellowState = LOW;

const int driverGreenPin = 10;
bool driverGreenState = HIGH;

const int passerRedPin = 6;
bool passerRedState = HIGH;

const int passerGreenPin = 5;
bool passerGreenState = LOW;

const int buzzerPin = 11;
unsigned int buzzerTone = 100;
bool buzzerState = LOW;
unsigned int buzzerDelay = 0;
unsigned int buzzerDuration = 0;

int counter = 0;


unsigned int debounceTimer = 0;
unsigned int debounceDelay = 50;

unsigned int changeStateOneDelay = 10000;
unsigned int changeStateTwoDelay = 3000;
unsigned int changeStateThreeDelay = 10000;
unsigned int changeStateFourDelay = 5000;

unsigned int newStateTimer = 0;
unsigned int blinkTimer = 0;
unsigned int buzzerTimer = 0;

unsigned int blinkDelay = 300;

int currentState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(driverRedPin, OUTPUT);
  pinMode(driverYellowPin, OUTPUT);
  pinMode(driverGreenPin, OUTPUT);
  pinMode(passerGreenPin, OUTPUT);
  pinMode(passerRedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // no state = start;
  // drivers: green && passers: red
  if(currentState == 0) {
    driverGreenState = HIGH;
    digitalWrite(driverGreenPin, driverGreenState);
    passerRedState = HIGH;
    digitalWrite(passerRedPin, passerRedState);
    
    reading = digitalRead(buttonPin);


    if(buttonState != prevReading) {
      debounceTimer = millis();
    }
    if( millis() - debounceTimer > debounceDelay) {
       if(reading != buttonState) {
          buttonState = reading;
          if(buttonState == LOW) {
            currentState = 1;
            newStateTimer = millis();
          }
       }
    }
   
    prevReading = buttonState;
  }


  // 1st State End = button pushed and then 10 seconds expired; 
  // drivers: yellow && passers: red

  if(currentState == 1 && millis() - newStateTimer > changeStateOneDelay) {
    driverGreenState = LOW;
    digitalWrite(driverGreenPin, driverGreenState);
    driverYellowState = HIGH;
    digitalWrite(driverYellowPin, driverYellowState);
    
    newStateTimer = millis();
    currentState = 2;
  }
  
  // 2nd State End = 3 seconds expired;
  // drivers: red && passers: green
  if(currentState == 2 && millis() - newStateTimer > changeStateTwoDelay) {
    driverYellowState = LOW;
    digitalWrite(driverYellowPin,driverYellowState);
    driverRedState = HIGH;
    digitalWrite(driverRedPin, driverRedState); 

    passerRedState = LOW;
    digitalWrite(passerRedPin, passerRedState);
    passerGreenState = HIGH;
    digitalWrite(passerGreenPin, passerGreenState);

    currentState = 3;
    newStateTimer = millis();
    buzzerTimer = millis();
    buzzerDuration = 100;
    buzzerDelay = 500;
  }

  // 3th State Start = use buzzer
  // 3th State End = 10 seconds expired;
  // drivers: red && passers: blinking green

  if(currentState == 3 && millis() - newStateTimer < changeStateThreeDelay)  {
    if(millis() - buzzerTimer > buzzerDelay) {
      buzzerState = !buzzerState;
      buzzerTimer = millis();
    }
    if(buzzerState == HIGH){
      tone(buzzerPin, buzzerTone, buzzerDuration);
    }
  }

  if(currentState == 3 && millis() - newStateTimer > changeStateThreeDelay) {
    currentState = 4;
    blinkTimer = millis();
    newStateTimer = millis();
    buzzerDuration = 50;
    blinkDelay = 250;
  }

 // 4th State Start = change semaphore status for passers
 // 4th State End = 5 seconds expired then Go Back To 1st State
 // drivers: red && passers: (blinking) green
 
 if(currentState == 4 && millis() - newStateTimer < changeStateFourDelay) {
  
    if(millis() - blinkTimer > blinkDelay) {
     passerGreenState = !passerGreenState;
     blinkTimer = millis();
    }
  
    if(passerGreenState == LOW) {
      digitalWrite(passerGreenPin, LOW);
    }
    
    else {
      digitalWrite(passerGreenPin, HIGH);
      tone(buzzerPin, buzzerTone, buzzerDuration);
    }
  
 }

 if(currentState == 4 && millis() - newStateTimer > changeStateFourDelay) {
    passerGreenState = LOW;
    digitalWrite(passerGreenPin, passerGreenState);
    driverRedState = LOW;
    digitalWrite(driverRedPin, driverRedState);
  
    currentState = 0;
    newStateTimer = millis();
 }
}
```

</details>
  
  </details>

---
