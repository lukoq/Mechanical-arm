#include <Servo.h>
#include <Arduino.h>
#include <math.h>

bool moveToCoords(float x, float y, float gamma);
int applyBetaLimits(int a, int requestedB);
void updateServos();

//PHYSICS
const float H = 6.5; 
const float L1 = 8.0; 
const float L2 = 8.0;

//TARGETS ARRAY {Distnace, Height, Rotation}
float targetsArr[][3] = {
  {12.0, 0.0, 40.0},
  {12.0, 0.0, 170.0},
  {5.0, 7.0, 90.0},
  {8.0, 1.0, 50.0},
  {8.0, 1.0, 160.0},
  {5.0, 7.0, 90.0},
  {5.0, 2.0, 60.0},
  {5.0, 2.0, 150.0},
  {5.0, 7.0, 90.0},
};

const int numTargets = sizeof(targetsArr) / sizeof(targetsArr[0]);
int currentTargetIndex = 0; 

float currentAlpha = 100.0;
float currentBeta = 135.0;
float currentGamma = 90.0;
int targetAlpha = 100;
int targetBeta = 135;
int targetGamma = 90;


const float stepSize = 0.5;      
unsigned long previousMillis = 0;
const unsigned long interval = 20; 
bool isWaiting = false;
unsigned long pauseMillis = 0;
const unsigned long pauseTime = 1500; 


Servo bottomServo, topServo, rotatorServo;

void setup() {
  Serial.begin(9600);
  bottomServo.attach(4);
  rotatorServo.attach(3);
  topServo.attach(2);

  bottomServo.write(currentAlpha);
  topServo.write(currentBeta);
  rotatorServo.write(currentGamma);
}


void updateServos() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    bool needsUpdate = false;

    //Bottom servo update
    if(abs(targetAlpha - currentAlpha) > stepSize) {
      if(currentAlpha < targetAlpha) {
         currentAlpha += stepSize;
      }
      else {
        currentAlpha -= stepSize;
      }
      needsUpdate = true;
    } 
    else {
      currentAlpha = targetAlpha; 
    }

    //Top servo update
    if(abs(targetBeta - currentBeta) > stepSize) {
      if(currentBeta < targetBeta){
         currentBeta += stepSize;
      }
      else {
        currentBeta -= stepSize;
      }
      needsUpdate = true;
    } 
    else {
      currentBeta = targetBeta;
    }

    //Rotator servo update
    if(abs(targetGamma - currentGamma) > stepSize) {
      if(currentGamma < targetGamma) {
        currentGamma += stepSize;
      }
      else {
        currentGamma -= stepSize;
      }
      needsUpdate = true;
    } 
    else {
      currentGamma = targetGamma;
    }


    if(needsUpdate) {
      int safeBeta = applyBetaLimits(round(currentAlpha), round(currentBeta));
      
      bottomServo.write(round(currentAlpha));
      topServo.write(safeBeta);
      rotatorServo.write(round(currentGamma));
    }
  }
}

int applyBetaLimits(int a, int requestedB) {
  int bMin, bMax;

  if(a <= 135) {
    bMin = 180 - a;
    bMax = 180;
  } 
  else if(a <= 170) {
    bMin = 40;
    bMax = map(a, 140, 170, 170, 140);
  } 
  else {
    bMin = map(a, 175, 180, 60, 70);
    bMax = map(a, 175, 180, 135, 130);
  }

  return constrain(requestedB, bMin, bMax);
}


bool moveToCoords(float x, float y, float gamma) {
  float relY = y - H;
  float distSq = x*x + relY*relY;
  float dist = sqrt(distSq);

  if(dist > (L1 + L2) || dist < abs(L1 - L2)) {
    return false;
  }

  float cosT2 = constrain((distSq - L1*L1 - L2*L2)/(2.0*L1 * L2), -1.0, 1.0);
  float t2 = acos(cosT2);
  
  float cosT1_part = constrain((L1*L1 + distSq - L2*L2)/(2.0*L1 * dist), -1.0, 1.0);

  float t1 = atan2(relY, x) + acos(cosT1_part);
  int a = round(190.0 - degrees(t1));

  float t2_horizon = t1 - t2; 
  int b = round(degrees(t2_horizon) + 135.0);

  int g = constrain(round(gamma), 0, 180);
  a = constrain(a, 45, 180);
  b = applyBetaLimits(a, b); 

  targetAlpha = a;     
  targetBeta = b;
  targetGamma = g;

  return true;
}

void loop() {
  updateServos();

  bool isReached = (abs(currentAlpha - targetAlpha) < 0.1) && 
                   (abs(currentBeta - targetBeta) < 0.1) && 
                   (abs(currentGamma - targetGamma) < 0.1);

  if(isReached) {
    if(!isWaiting) {
      isWaiting = true;
      pauseMillis = millis();
      Serial.print("Osiagnieto punkt nr: ");
      Serial.println(currentTargetIndex);
    } 
    else {
      if(millis() - pauseMillis >= pauseTime) {
        isWaiting = false;
        currentTargetIndex++;
        
        if(currentTargetIndex >= numTargets) {
          currentTargetIndex = 0; 
        }

        float nextX = targetsArr[currentTargetIndex][0];
        float nextY = targetsArr[currentTargetIndex][1];
        float nextGamma = targetsArr[currentTargetIndex][2];
        
        bool success = moveToCoords(nextX, nextY, nextGamma);
        if (!success) {
          Serial.println("Error: Beyond the limit!");
        }
      }
    }
  }
}