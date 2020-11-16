#include <Servo.h>

constexpr int servo0_pin = 5;
constexpr int servo1_pin = 6;
constexpr int servo2_pin = 9;
constexpr int servo3_pin = 10;

constexpr int interval = 10;
constexpr float cheat_factor = 0;
constexpr int MOTION_TIME = 2000;

int pos[4] = {10, 140, 180, 180};

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   

void move_gently(int final_pos [4])
{
  // Cheat a bit first
  for (int i {0}; i < 4; i++)
  {
    float difference = final_pos[i] - pos[i];
    float temp = difference * cheat_factor + pos[i];
    pos[i] = temp;
  }

  Serial.print("Jump: "); Serial.print(pos[0]); Serial.print(" "); 
  Serial.print(pos[1]); Serial.print(" "); Serial.print(pos[2]);
  Serial.print(" "); Serial.println(pos[3]);
  
  while (pos[0] != final_pos[0] || pos[1] != final_pos[1] || pos[2] != final_pos[2] || pos[3] != final_pos[3])
  {    
    for (int i {0}; i < 4; i++)
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
    delay(interval);   
    servo0.write(pos[0]);
    servo1.write(pos[1]);
    servo2.write(pos[2]);
    servo3.write(pos[3]);
    Serial.print("Moving: "); Serial.print(pos[0]); Serial.print(" ");
    Serial.print(pos[1]); Serial.print(" "); Serial.print(pos[2]);
    Serial.print(" "); Serial.println(pos[3]); 
  }
}

void move_gently_2(int final_pos [4]) {
  int motion_time = MOTION_TIME;
  int motion_start = millis();

  int start_pos_arr [4] {};
  for (int i {0}; i < 4; ++i)
  {
    start_pos_arr[i] = pos[i];
  }

  while (pos[0] != final_pos[0] || pos[1] != final_pos[1] || pos[2] != final_pos[2] || pos[3] != final_pos[3])
  {
    for (int i {0}; i < 4; ++i) 
    {
      int current_time = millis() - motion_start;
      int angle = motion_curve(start_pos_arr[i], final_pos[i], motion_time, current_time);
      pos[i] = angle;
    } 
    servo0.write(pos[0]);
    servo1.write(pos[1]);
    servo2.write(pos[2]);
    servo3.write(pos[3]);
    Serial.print("Moving: "); Serial.print(pos[0]); Serial.print(" ");
    Serial.print(pos[1]); Serial.print(" "); Serial.print(pos[2]);
    Serial.print(" "); Serial.println(pos[3]);
  }
}

float motion_curve(int start_pos, int end_pos, int motion_time, int current_time)
{
  if (current_time > motion_time)
  {
    return end_pos;
  }
  int a = start_pos;
  int b = end_pos;
  int t = motion_time;
  int x = current_time;

  float c = (b-a) / 2;
  float result = c * (cbrt((x - (t/2.0))/(t/2.0))) + c + a;
  return result;
}

void setup() 
{
  Serial.begin(9600);
  servo0.attach(servo0_pin);
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
  servo3.attach(servo3_pin);

  servo0.write(pos[0]);
  servo1.write(pos[1]);
  servo2.write(pos[2]);
  servo3.write(pos[3]);

  Serial.println("READY");
}

void loop() 
{
  if (Serial.available() > 0) {
    int angle_0 = Serial.parseInt();
    int angle_1 = Serial.parseInt();
    int angle_2 = Serial.parseInt();
    int angle_3 = Serial.parseInt();
    Serial.print("Angle 0: ");
    Serial.println(angle_0);
    Serial.print("Angle 1: ");
    Serial.println(angle_1);
    Serial.print("Angle 2: ");
    Serial.println(angle_2); 
    Serial.print("Angle 3: ");
    Serial.println(angle_3); 
    
    // serialFlush();
    int new_pos [4] {angle_0, angle_1, angle_2, angle_3};
    move_gently_2(new_pos);
  }
}
