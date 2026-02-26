#include <Servo.h>
#include <Arduino.h>

//SERVO OBJECTS
Servo topServo;
Servo bottomServo;
Servo rotatorServo;

//PINS
const int topPin = 2;
const int rotatorPin = 3;
const int bottomPin = 4;

//TIME CONTROL
unsigned long previousMillis = 0;
const unsigned long interval = 50; 

//SERVO POSITIONS
int bottomPos = 90;
int topPos = 120;
int rotatorPos = 90;

int bottomDirection = 1; 
int rotatorDirection = 1; 

//PHYSICS
const float H = 6.5;   
const float L1 = 8.0;  
const float L2 = 10.0; 
float targetHeight = 5.0;

void setup() {
  Serial.begin(9600);

  topServo.attach(topPin);
  bottomServo.attach(bottomPin);
  rotatorServo.attach(rotatorPin);
  topServo.write(topPos);
  bottomServo.write(bottomPos);
  rotatorServo.write(rotatorPos);
}

int applyLimits(int a, int requestedB) {
  int bMin, bMax;

  if (a <= 135) {
    bMin = 180 - a;
    bMax = 180;
  } 
  else if (a <= 170) {
    bMin = 40;
    bMax = map(a, 140, 170, 170, 140);
  } 
  else {
    bMin = map(a, 175, 180, 60, 70);
    bMax = map(a, 175, 180, 135, 130);
  }

  return constrain(requestedB, bMin, bMax);
}

int calculateBeta(float y, int a) {

  float theta1 = (195.0 - a) * PI / 180.0;
  float sinTheta2 = (y - H - L1 * sin(theta1)) / L2;
  
  if(sinTheta2 < -1.0 || sinTheta2 > 1.0){
    return -1;
  } 
  
  float theta2 = asin(sinTheta2);
  int b = round(degrees(theta2) + 135.0);
  
  return b;
}


void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    bottomPos += bottomDirection;
    rotatorPos += rotatorDirection;

    //Range
    if(bottomPos >= 170 || bottomPos <= 70) {
      bottomDirection *= -1; 
    }
    if(rotatorPos >= 140 || rotatorPos <= 40) {
      rotatorDirection *= -1; 
    }

    int rawBeta = calculateBeta(targetHeight, bottomPos);
    
    if (rawBeta != -1){
      topPos = applyLimits(bottomPos, rawBeta);
      
      bottomServo.write(bottomPos);
      topServo.write(topPos);
      rotatorServo.write(rotatorPos);
 
    }
  }
}