#include <EEPROM.h>

//shift register pins
const int dataPin = 12; //DS
const int latchPin = 11; // STCP
const int clockPin = 10; // SHCP

//4 digits 7-segment display pins
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayRange = 4;
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};


int currentDigit = 0;
int currentDigitsValues[] = {
  0, 0, 0, 0
};

//joystick
const int VRx = A0;
const int VRy = A1;
const int SW = 3;
const int minThreshold = 300;
const int maxThreshold = 700;
int xPosition = 0;
int yPosition = 0;
bool isMoved = false;

volatile bool buttonPressed = LOW;
bool lastButtonState = LOW;
volatile unsigned long lastPress = 0;
const int debounceTime = 250;

//blinking DP variables
unsigned long blinkTimer = 0;
const int waitTime = 400;
const int resetBlinkTimer = 800;

//
const int eepromAddress = 0;

int digitArray[17] = {
  //A B C D E F G DP
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);
  for (int i = 0; i < displayRange; i++)
    pinMode(displayDigits[i], OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SW), pressJoystickButton, FALLING);
  readArrayFromEPPROM(eepromAddress, currentDigitsValues, 4);

}

void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  
  // first state: select which place
  if (buttonPressed == LOW) {
    // save current state when returning from next state
    if (buttonPressed != lastButtonState) {
      writeArrayIntoEEPROM(eepromAddress, currentDigitsValues, 4);
      lastButtonState = buttonPressed;
    }
    changeDisplayDigitPosition();
  }
  //second state: select new digit value
  else {
    lastButtonState = HIGH;
    changeDisplayDigitValue();
  }

  displayData();

}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void showDigit(int displayNumber) {
  for (int i = 0; i < displayRange; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void displayData() {
  for ( int i = 0; i < displayRange; i++) {
    showDigit(i);
    //1st state: show blinking DP
    if (i == currentDigit && buttonPressed == LOW) {
      if (millis() - blinkTimer < waitTime) {
        writeReg(digitArray[currentDigitsValues[i]]);
      }
      if (millis() - blinkTimer > waitTime) {
        // first bit of digitArray is for enabling DP(or disabling)
        writeReg(digitArray[currentDigitsValues[i]] + 1);
      }
      if (millis() - blinkTimer > resetBlinkTimer) {
        blinkTimer = millis();
      }
    }
  
    else {
      writeReg(digitArray[currentDigitsValues[i]]);
    }

    // if the button is pressed, the DP led will be set to HIGH
    if (i == currentDigit && buttonPressed == HIGH) {
      writeReg(digitArray[currentDigitsValues[i]] + 1);
    }
    delay(5);
  }
}

void changeDisplayDigitValue() {
 
  if (yPosition >= maxThreshold && isMoved == false) {
    isMoved = true;
    currentDigitsValues[currentDigit]++;
    if (currentDigitsValues[currentDigit] > 9) {
      currentDigitsValues[currentDigit] = 0;
    }
  }
  
  if (yPosition <= minThreshold && isMoved == false) {
    isMoved = true;
    currentDigitsValues[currentDigit]--;
    if (currentDigitsValues[currentDigit] < 0) {
      currentDigitsValues[currentDigit] = 9;
    }
  }


  if (yPosition >= minThreshold && yPosition <= maxThreshold) {
    isMoved = false;
  }
}


void changeDisplayDigitPosition() {
  if (xPosition >= maxThreshold && isMoved == false) {
    isMoved = true;
    currentDigit++;
    if (currentDigit > 3) {
      currentDigit = 3;
    }
  }
  
  if (xPosition <= minThreshold && isMoved == false) {
    isMoved = true;
    currentDigit--;
    if (currentDigit < 0) {
      currentDigit = 0;
    }
  }

  if (xPosition >= minThreshold && xPosition <= maxThreshold) {
    isMoved = false;
  }
}

void pressJoystickButton() {
  if (millis() - lastPress > debounceTime) {
    buttonPressed = !buttonPressed;
  }
  lastPress = millis();
}

// function for writing an array into EEPROM
void writeArrayIntoEEPROM(int address, int numbers[], int arraySize) {
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++) {
  // int number is using 2 bytes
    byte firstByte =  numbers[i] >> 8;
    byte secondByte = numbers[i] & 0xFF;
    // write the bytes on consecutive address
    EEPROM.write(addressIndex, firstByte);
    EEPROM.write(addressIndex + 1, secondByte);
    addressIndex += 2;
  }
}


void readArrayFromEPPROM(int address, int numbers[], int arraySize) {
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++) {
    byte firstByte = EEPROM.read(addressIndex) << 8;
    byte secondByte = EEPROM.read(addressIndex + 1);
    numbers[i] = firstByte + secondByte;
  // if the memory is not allocated initialize with 0
    if(numbers[i] < 0)
      numbers[i] = 0;
  
    addressIndex += 2;
  }
}
