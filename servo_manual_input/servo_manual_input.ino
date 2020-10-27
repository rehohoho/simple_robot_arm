#include <Servo.h>

const int servo1_pin = 5;
const int servo2_pin = 6;
const int servo3_pin = 9;

int pos[3] = {10, 140, 180};

Servo servo1;
Servo servo2;
Servo servo3;

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   

void move_gently(int final_pos [3])
{
  while (pos[0] != final_pos[0] || pos[1] != final_pos[1] || pos[2] != final_pos[2])
  {
    for (int i {0} ; i < 3; i++)
    {
      if (pos[i] != final_pos[i])
      {
       if (final_pos[i] > pos[i])
       {
        pos[i] += 1;
       }
       else
       {
        pos[i] -= 1;
       }
      } 
    }
    delay(10);   
    servo1.write(pos[0]);
    servo2.write(pos[1]);
    servo3.write(pos[2]);
    Serial.print("pos 1: ");
    Serial.println(pos[0]);
    Serial.print("pos 2: ");
    Serial.println(pos[1]);
    Serial.print("pos 3: ");
    Serial.println(pos[2]); 
  }
}


void setup() {
  Serial.begin(9600);
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
  servo3.attach(servo3_pin);

  servo1.write(pos[0]);
  servo2.write(pos[1]);
  servo3.write(pos[2]);
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
    int new_pos [3] {angle_1, angle_2, angle_3};
    move_gently(new_pos);
  }
}
