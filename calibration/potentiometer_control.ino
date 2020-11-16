/*
 * Potentiometer Control
 * 
 * Wiring
 * - A0: Potentiometer 
 * - Pin 2: Button (Select active servo) [Interrupt required]
 * - Pin 3: Button (Enable/Disable editing) [Interrupt required]
 * - Pin 5: Servo 0 signal [PWM required]
 * - Pin 6: Servo 1 signal [PWM required]
 * - Pin 7: Servo 2 signal [PWM required]
 * - Pin 8: Servo 3 signal [PWM required]
 * - Pin 13, 12, 8, 7: LED (Optional - Current servo will also be shown on serial monitor)
 * 
 * Example serial monitor output
 * 
 *    Servo 3 Editing: 0  Angle: 178.42 75 76 130 178
 */


#include <Servo.h>

constexpr int sensor_pin {A0};
constexpr int button_pin {2};
constexpr int enable_edit_pin {3};
constexpr int servo_0_pin {5};
constexpr int servo_1_pin {6};
constexpr int servo_2_pin {9};
constexpr int servo_3_pin {10};
constexpr int led_pins[4] = {13, 12, 8, 7};
constexpr int update_interval {50};

// Default servo positions
int servo_pos[4] {0, 0, 0, 0};
int last_check_millis {0};

volatile int current_servo {0};
volatile int last_change_millis {0};
volatile int last_edit_millis {0};
volatile int enable_edit {0};

Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;

void isr()
{
  if (millis() - last_change_millis > 200)
  {
    current_servo = (current_servo + 1) % 4;
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

void on_led(int led_on)
{
  for (int i {0}; i < 4; i++)
  {
   if (i != led_on)
   {
     digitalWrite(led_pins[i], LOW);
   } 
   else {
     digitalWrite(led_pins[i], HIGH);
   }
  }
}

void write_servo_pos()
{
    servo_0.write(servo_pos[0]);
    servo_1.write(servo_pos[1]);
    servo_2.write(servo_pos[2]);
    servo_3.write(servo_pos[3]);
}

void setup()
{
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(enable_edit_pin, INPUT_PULLUP);
  pinMode(sensor_pin, INPUT);
  for (auto i: led_pins)
  {
   pinMode(i, OUTPUT); 
  }
  servo_0.attach(servo_0_pin);
  servo_1.attach(servo_1_pin);
  servo_2.attach(servo_2_pin);
  servo_3.attach(servo_3_pin);
  Serial.begin(9600);
  
  attachInterrupt(digitalPinToInterrupt(button_pin), isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(enable_edit_pin), isr_2, FALLING);
}

void loop()
{
  // Get data from potentiometer
  float angle {};
  int sensor_input = analogRead(sensor_pin);
  angle = sensor_input / 1023.0 * 180;
  // Serial.print("Current angle: "); Serial.println(angle);
    
  // Update angle from potentiometer
  if (millis() - last_check_millis > update_interval && enable_edit)
  {
    servo_pos[current_servo] = angle;
    last_check_millis = millis();
  }
  
  on_led(current_servo);
  write_servo_pos();
  Serial.print("Servo "); Serial.print(current_servo); Serial.print("\t");
  Serial.print("Editing: "); Serial.print(enable_edit); Serial.print("\t");
  Serial.print("Angle: "); Serial.print(angle); Serial.print("\t");
  for (auto i: servo_pos)
  {
    Serial.print(i);
    Serial.print(" ");
  }
  Serial.println();
}
