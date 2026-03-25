#include <Servo.h>
#include <Arduino.h>

//SERVO OBJECTS
Servo grapplerServo;

//PINS
const int grapplerPin = 2;

//TIME CONTROL
unsigned long previousMillis = 0;
const unsigned long interval = 50; 

//SERVO POSITIONS
int grapplerPos = 90;

void setup() {
  Serial.begin(9600);

  grapplerServo.attach(grapplerPin);
  grapplerServo.write(grapplerPos);

  Serial.println("MANUAL CALIBRATION SYSTEM");
  Serial.println("Commands: [w/s] Grappler");
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
          grapplerPos += 5;    
          moved = true; 
          break;
        }
        case 's': {
          grapplerPos -= 5;
          moved = true;
          break;
        }
      }

      if(moved) {
        grapplerPos = constrain(grapplerPos, 0, 180);

        //Update servo position
        grapplerServo.write(grapplerPos); // RANGE: 110 to 180


        //Print status 
        Serial.print("DATA: GRAPPLER: ");
        Serial.println(grapplerPos);
      }
    }
  }
}