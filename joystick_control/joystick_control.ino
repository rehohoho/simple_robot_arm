/*
 * Joystick Control
 * 
 * Wiring
 * - A0: VRX
 * - A1: VRY
 * - Pin 2: SW [Interrupt required]
 * - Pin 3: Button (Enable/Disable editing) [Interrupt required]
 * - Pin 5: Servo 0 signal [PWM required]
 * - Pin 6: Servo 1 signal [PWM required]
 * - Pin 9: Servo 2 signal [PWM required]
 * - Pin 10: Servo 3 signal [PWM required]
 * 
 * Example serial monitor output
 * 
 *    Servo 2(x) and 3(y)  Editing: 1  18 15 0 0
 */


#include <Servo.h>

constexpr int x_pin {A0};
constexpr int y_pin {A1};
constexpr int joystick_btn_pin {2};
constexpr int button_pin {3};
constexpr int servo_0_pin {5};
constexpr int servo_1_pin {6};
constexpr int servo_2_pin {9};
constexpr int servo_3_pin {10};

int servo_pos[4] {0, 180, 0, 90};
int last_change_millis {0};
int last_edit_millis {0};

volatile int current_servo {0};
volatile int enable_edit {0};

Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;

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

void setup() 
{
  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(joystick_btn_pin, INPUT_PULLUP);
  pinMode(button_pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button_pin), isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(joystick_btn_pin), isr_2, FALLING);

  servo_0.attach(servo_0_pin);
  servo_1.attach(servo_1_pin);
  servo_2.attach(servo_2_pin);
  servo_3.attach(servo_3_pin);
    
  Serial.begin(9600);
}

void loop() 
{
  int x_input {analogRead(A0)};
  int y_input {analogRead(A1)};
  int btn_input {digitalRead(2)};
//  Serial.print(x_input);
//  Serial.print(" ");
//  Serial.print(y_input);
//  Serial.print(" ");
//  Serial.println(btn_input);
//  Serial.println(servo_pos);

  if (x_input > 900 && enable_edit)
  {
    if (servo_pos[current_servo] < 180)
      servo_pos[current_servo] += 1; 
  }
  else if (x_input < 300 && enable_edit)
  {
    if (servo_pos[current_servo] > 0)
      servo_pos[current_servo] -= 1;
  }
  
  if (y_input < 300 && enable_edit)
  {
    if (servo_pos[current_servo + 1] < 180)
      servo_pos[current_servo + 1] += 1; 
  }
  else if (y_input > 900 && enable_edit)
  {
    if (servo_pos[current_servo + 1] > 0)
      servo_pos[current_servo + 1] -= 1;
  }
  

  servo_0.write(servo_pos[0]);
  servo_1.write(servo_pos[1]);
  servo_2.write(servo_pos[2]);
  servo_3.write(servo_pos[3]);
  Serial.print("Servo "); Serial.print(current_servo); Serial.print("(x) and "); Serial.print(current_servo + 1); Serial.print("(y)"); Serial.print("\t");
  Serial.print("Editing: "); Serial.print(enable_edit); Serial.print("\t"); 
  Serial.print(servo_pos[0]); Serial.print(" "); Serial.print(servo_pos[1]); Serial.print(" ");
  Serial.print(servo_pos[2]); Serial.print(" "); Serial.println(servo_pos[3]);
}
