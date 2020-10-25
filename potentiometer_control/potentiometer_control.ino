#include <Servo.h>

constexpr int sensor_pin = A0;
constexpr int button_pin = 2;
constexpr int servo_0_pin = 3;
constexpr int servo_1_pin = 5;
constexpr int servo_2_pin = 6;
constexpr int servo_3_pin = 10;
constexpr int led_pins[4] = {13, 12, 8, 7};
constexpr int update_interval = 100;

volatile int current_servo = 0;

int servo_pos[4] = {0, 0, 0, 0};
int last_check_millis = 0;

Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;

void isr()
{
  current_servo = (current_servo + 1) % 4;
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
}

void loop()
{
  // Update angle from potentiometer
  if (millis() - last_check_millis > update_interval)
  {
    int sensor_input = analogRead(sensor_pin);
    float angle = sensor_input / 1023.0 * 180;
    Serial.print("Current angle: "); Serial.println(angle);
    servo_pos[current_servo] = angle;
    last_check_millis = millis();
  }
  
  on_led(current_servo);
  write_servo_pos();
  for (auto i: servo_pos)
  {
    Serial.print(i);
    Serial.print(" ");
  }
  Serial.println();
}
