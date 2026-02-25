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

void setup() {
  Serial.begin(9600);

  topServo.attach(topPin);
  bottomServo.attach(bottomPin);
  rotatorServo.attach(rotatorPin);
  topServo.write(topPos);
  bottomServo.write(bottomPos);
  rotatorServo.write(rotatorPos);

  Serial.println("MANUAL CALIBRATION SYSTEM");
  Serial.println("Commands: [w/s] Top | [a/d] Bottom | [q/e] Rotator");
  Serial.println("1 step == 5 degrees");
}

void loop() {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if(Serial.available() > 0) {
      char sign = Serial.read();
      bool moved = false;

      //Logic 
      switch(sign) {
        case 'w': {
          topPos += 5;    
          moved = true; 
          break;
        }
        case 's': {
          topPos -= 5;
          moved = true;
          break;
        }
        case 'd': {
          bottomPos += 5; 
          moved = true; 
          break;
        }
        case 'a':{
          bottomPos -= 5;
          moved = true;
          break;
        }
        case 'e': { 
          rotatorPos += 5;
          moved = true;
          break;
        }
        case 'q': {
          rotatorPos -= 5;
           moved = true;
           break;
        }
      }

      if(moved) {
        topPos = constrain(topPos, 0, 180);
        bottomPos = constrain(bottomPos, 0, 180);
        rotatorPos = constrain(rotatorPos, 0, 180);

        //Update servo positions
        topServo.write(topPos);
        bottomServo.write(bottomPos);
        rotatorServo.write(rotatorPos);

        //Print status 
        Serial.print("DATA: BOTTOM: ");
        Serial.print(bottomPos);
        Serial.print(", TOP: ");
        Serial.println(topPos);
      }
    }
  }
}