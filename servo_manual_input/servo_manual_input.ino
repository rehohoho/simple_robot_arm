#include <Servo.h>

const int servo1_pin = 3;
const int servo2_pin = 5;
const int servo3_pin = 6;

Servo servo1;
Servo servo2;
Servo servo3;

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   

void setup() {
  Serial.begin(9600);
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
  servo3.attach(servo3_pin);
}

void loop() {
  if (Serial.available() > 0) {
    int angle_1 = Serial.parseInt();
    int angle_2 = Serial.parseInt();
    int angle_3 = Serial.parseInt();
    Serial.print("Angle 1: ");
    Serial.println(angle_1);
    Serial.print("Angle 2: ");
    Serial.println(angle_2);
    Serial.print("Angle 3: ");
    Serial.println(angle_3); 
    
    serialFlush();
    
    servo1.write(angle_1);
    servo2.write(angle_2);
    servo3.write(angle_3);
  }
}
