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