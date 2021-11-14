const int samples = 1000;
int arrayOfSamples[samples];  
unsigned long averaging = 0;

int antenaPin = 2;
int antenaRead = 0;

const int buzzerPin = 10;
int buzzerTone = 100;
int buzzerDuration = 14;

// declare all the segments pins
const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;

const int segSize = 8;
const int noOfDigits = 10;

bool dpState = LOW;
int numberToDisplay = 0;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};



byte digitMatrix[noOfDigits][segSize - 1] = {
  // a b c d e f g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}



void setup() {
  Serial.begin(9600);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
}
void loop() {

  for(int i = 0; i < samples; i++) {
    arrayOfSamples[i] = analogRead(antenaPin);
    averaging += arrayOfSamples[i];
  }
  antenaRead = averaging / samples;
  antenaRead = constrain(antenaRead, 1, 100);
  antenaRead = map(antenaRead, 1, 100, 1, 255);
  buzzerTone = map(antenaRead, 1, 255, 1, 255 * 15);
  tone(buzzerPin, buzzerTone, buzzerDuration);
  numberToDisplay = map(antenaRead, 1, 255, 1, 9);
  dpState = LOW;
  Serial.println(antenaRead);
  displayNumber(numberToDisplay, dpState);
  averaging = 0;
  delay(100);
}