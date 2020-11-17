/*
 * Challenge code
 * 
 * Joystick control + hard-coded sequence
 * 
 * Wiring
 * - A0: VRX
 * - A1: VRY
 * - Pin 2: SW [Interrupt required]
 * - Pin 3: Button (Change joystick servo) [Interrupt required]
 * - Pin 4: Start button (start hardcoded sequence)
 * - Pin 5: Servo 0 signal [PWM required]
 * - Pin 6: Servo 1 signal [PWM required]
 * - Pin 7: Mode button (toggle between joystick/hardcode)
 * - Pin 9: Servo 2 signal [PWM required]
 * - Pin 10: Servo 3 signal [PWM required]
 */


#include <Servo.h>

constexpr int servo0_pin {5};
constexpr int servo1_pin {6};
constexpr int servo2_pin {9};
constexpr int servo3_pin {10};
constexpr int start_btn {4};

constexpr int x_pin {A0};
constexpr int y_pin {A1};
constexpr int joystick_btn_pin {2};
constexpr int button_pin {3};
constexpr int mode_pin {7};

constexpr int interval {10};
constexpr float cheat_factor {0};
constexpr int TURN_ANGLE {20};
constexpr int MOTION_TIME {3000};
constexpr int CLAW_CLOSE_TIME {2000};
constexpr int CLAW_CLOSE_ANGLE {100};
constexpr int CLAW_OPEN_ANGLE {0};

int last_change_millis {0};
int last_edit_millis {0};

volatile int current_servo {0};
volatile int enable_edit {0};

int pos[4] {0, 180, 0, CLAW_OPEN_ANGLE};

int pick_up_offset [4] {0, 0, 0, 0};
int main_offset [4] {0, 0, 0, 0};

int home_pos[4] {0, 180, 0, CLAW_OPEN_ANGLE};
int home_pos_close[4] {home_pos[0], home_pos[1], home_pos[2], CLAW_CLOSE_ANGLE};
int pick_up_pos[4] {18 + pick_up_offset[0], 75 + pick_up_offset[1], 0 + pick_up_offset[2], CLAW_OPEN_ANGLE + pick_up_offset[3]};
int level_1[4] {26 + main_offset[0], 103 + main_offset[1], 0 + main_offset[2], CLAW_CLOSE_ANGLE + main_offset[3]}; 
int level_2[4] {22 + main_offset[0], 110 + main_offset[1], 0 + main_offset[2], CLAW_CLOSE_ANGLE}+ main_offset[3]; 
int level_3[4] {20 + main_offset[0], 116 + main_offset[1], 0 + main_offset[2], CLAW_CLOSE_ANGLE}+ main_offset[3]; 
int level_4[4] {19 + main_offset[0], 121 + main_offset[1], 0 + main_offset[2], CLAW_CLOSE_ANGLE}+ main_offset[3]; 
int level_5[4] {97 + main_offset[0], 120 + main_offset[1], 0 + main_offset[2], CLAW_CLOSE_ANGLE}+ main_offset[3]; 
int level_6[4] {97 + main_offset[0], 120 + main_offset[1], 0 + main_offset[2], CLAW_CLOSE_ANGLE}+ main_offset[3];  

int mode {0};
int last_mode_millis {0};


Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

void isr()
{
  if (millis() - last_change_millis > 200)
  {
    current_servo = (current_servo + 2) % 4;
    last_change_millis = millis();
  }
}

void isr_2()
{
  if (millis() - last_edit_millis > 200)
  {
    enable_edit = !enable_edit;
    last_edit_millis = millis();    
  }
}

void serialFlush()
{
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   

void move_gently_2(int final_pos [4], int motion_time = MOTION_TIME) 
{
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
  int temp_pos[4] {pick_up_pos[0], pick_up_pos[1], pick_up_pos[2] + 20, pick_up_pos[3]};
  move_gently_2(temp_pos);
  move_gently_2(pick_up_pos);  
}

void base() {
  Serial.println("---Homing---");
  move_gently_2(home_pos); 
}

void base_close() {
  Serial.println("---Homing---");
  move_gently_2(home_pos_close); 
}

void turn_abit() {
  Serial.println("---Turning---");
  int grip_pos[4] {pos[0], pos[1], pos[2] + TURN_ANGLE, pos[3]};
  move_gently_2(grip_pos);  
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

  pinMode(start_btn, INPUT_PULLUP);
  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(joystick_btn_pin, INPUT_PULLUP);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(mode_pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button_pin), isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(joystick_btn_pin), isr_2, FALLING);

  Serial.println("READY. Waiting for serial input to start.");
}

void loop() 
{
  if (!digitalRead(mode_pin) && millis() - last_mode_millis > 1000)
  {
    mode = !mode;
    last_mode_millis = millis();
  }
  
  if (mode == 0)
  {
    Serial.println("...");
    // Start program on serial input
    if (Serial.available() > 0 or digitalRead(start_btn) == 0) {
      Serial.println("---STARTING---");
      serialFlush();

  //TEMP CODE FOR TESTING
      grip();
      base_close();
      Serial.println("---LVL 1---");
      move_gently_2(level_1);
      ungrip();
      turn_abit();
      base();

      grip();
      base_close();
      Serial.println("---LVL 2---");
      move_gently_2(level_2);
      ungrip();
      turn_abit();
      base();

      grip();
      base_close();
      Serial.println("---LVL 3---");
      move_gently_2(level_3);
      ungrip();
      turn_abit();
      base();

      grip();
      base_close();
      Serial.println("---LVL 4---");
      move_gently_2(level_4);
      ungrip();
      turn_abit();
      base();
  

//      pickup();
//      grip();
//      base_close();
//      Serial.println("---LVL 1---");
//      move_gently_2(level_1);
//      ungrip();
//      turn_abit();
//      base();
      
//      pickup();
//      grip();
//      base_close();
//      Serial.println("---LVL 2---");
//      move_gently_2(level_2);
//      ungrip();
//      turn_abit();
//      base();
  
  //    pickup();
  //    grip();
  //    Serial.println("---LVL 3---");
  //    move_gently_2(level_3);
  //    ungrip();
  //    base();
  
  //    pickup();
  //    grip();
  //    Serial.println("---LVL 4---");
  //    move_gently_2(level_4);
  //    ungrip();
  //    base();
  
  //    pickup();
  //    grip();
  //    Serial.println("---LVL 5---");
  //    move_gently_2(level_5);
  //    ungrip();
  //    base();
  
  //    pickup();
  //    grip();
  //    Serial.println("---LVL 6---");
  //    move_gently_2(level_6);
  //    ungrip();
  //    base();
    }
  }
  else {
    int x_input {analogRead(A0)};
    int y_input {analogRead(A1)};
    int btn_input {digitalRead(2)};
  //  Serial.print(x_input);
  //  Serial.print(" ");
  //  Serial.print(y_input);
  //  Serial.print(" ");
  //  Serial.println(btn_input);
  //  Serial.println(pos);
  
    if (x_input > 900 && enable_edit)
    {
      if (pos[current_servo] < 180)
        pos[current_servo] += 1; 
    }
    else if (x_input < 300 && enable_edit)
    {
      if (pos[current_servo] > 0)
        pos[current_servo] -= 1;
    }
    
    if (y_input < 300 && enable_edit)
    {
      if (pos[current_servo + 1] < 180)
        pos[current_servo + 1] += 1; 
    }
    else if (y_input > 900 && enable_edit)
    {
      if (pos[current_servo + 1] > 0)
        pos[current_servo + 1] -= 1;
    }
    
  
    servo0.write(pos[0]);
    servo1.write(pos[1]);
    servo2.write(pos[2]);
    servo3.write(pos[3]);
    Serial.print("Servo "); Serial.print(current_servo); Serial.print("(x) and "); Serial.print(current_servo + 1); Serial.print("(y)"); Serial.print("\t");
    Serial.print("Editing: "); Serial.print(enable_edit); Serial.print("\t"); 
    Serial.print(pos[0]); Serial.print(" "); Serial.print(pos[1]); Serial.print(" ");
    Serial.print(pos[2]); Serial.print(" "); Serial.println(pos[3]);
   }
}
