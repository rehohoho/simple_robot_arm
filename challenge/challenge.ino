#include <Servo.h>

constexpr int servo0_pin {5};
constexpr int servo1_pin {6};
constexpr int servo2_pin {9};
constexpr int servo3_pin {10};

constexpr int interval {10};
constexpr float cheat_factor {0};
constexpr int MOTION_TIME {2000};
constexpr int CLAW_CLOSE_TIME {1000};
constexpr int CLAW_CLOSE_ANGLE {90};
constexpr int CLAW_OPEN_ANGLE {180};

int pos[4] {10, 140, 180, CLAW_OPEN_ANGLE};

int home_pos[4] {10, 140, 180, CLAW_OPEN_ANGLE};
int pick_up_pos[4] {90, 110, 180, CLAW_OPEN_ANGLE};
int level_1[4] {97, 120, 80, CLAW_CLOSE_ANGLE}; 
int level_2[4] {97, 120, 80, CLAW_CLOSE_ANGLE}; 
int level_3[4] {83, 125, 80, CLAW_CLOSE_ANGLE}; 
int level_4[4] {97, 120, 80, CLAW_CLOSE_ANGLE}; 
int level_5[4] {97, 120, 80, CLAW_CLOSE_ANGLE}; 
int level_6[4] {97, 120, 80, CLAW_CLOSE_ANGLE}; 


Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   

void move_gently_2(int final_pos [4], int motion_time = MOTION_TIME) {
  Serial.print("Moving to: "); Serial.print(final_pos[0]); Serial.print(" ");
  Serial.print(final_pos[1]); Serial.print(" "); Serial.print(final_pos[2]);
  Serial.print(" "); Serial.println(final_pos[3]);
    
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
    
//    Serial.print("Moving: "); Serial.print(pos[0]); Serial.print(" ");
//    Serial.print(pos[1]); Serial.print(" "); Serial.print(pos[2]);
//    Serial.print(" "); Serial.println(pos[3]);
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

void grip() {
  Serial.println("---Gripping---");
  int grip_pos[4] {pos[0], pos[1], pos[2], CLAW_CLOSE_ANGLE};
  move_gently_2(grip_pos, CLAW_CLOSE_TIME);
}

void ungrip() {
  Serial.println("---Releasing---");
  int grip_pos[4] {pos[0], pos[1], pos[2], CLAW_OPEN_ANGLE};
  move_gently_2(grip_pos, CLAW_CLOSE_TIME);
}

void pickup() {
  Serial.println("---Picking up---");
  move_gently_2(pick_up_pos);  
}

void base() {
  Serial.println("---Homing---");
  move_gently_2(home_pos); 
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

  Serial.println("READY. Waiting for serial input to start.");
}

void loop() 
{
  // Start program on serial input
  if (Serial.available() > 0) {
    Serial.println("---STARTING---");
    serialFlush();

    
    pickup();
    grip();
    Serial.println("---LVL 1---");
    move_gently_2(level_1);
    ungrip();
    base();
    
    pickup();
    grip();
    Serial.println("---LVL 2---");
    move_gently_2(level_2);
    ungrip();
    base();
    
  }
}
