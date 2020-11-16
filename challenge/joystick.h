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

int servo_pos[4] {0, 180, 0, 0};
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

