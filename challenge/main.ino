#include <Servo.h>
// #include <joystick.h>

constexpr int x_pin {A0};
constexpr int y_pin {A1};
constexpr int joystick_btn_pin {2};

int servo_pos[4] {0, 0, 180, 0};
int button1 = 3;
int button2 = A5;

const int YAW_SERVO_PIN = 9;
const int LOWER_TILT_SERVO_PIN = 5;
const int UPPER_TILT_SERVO_PIN = 6;
const int GRIP_SERVO_PIN = 10;

Servo yawServo;
Servo lowerTiltServo;
Servo upperTiltServo;
Servo gripServo;


void initServo() {
    yawServo.attach(YAW_SERVO_PIN);
    lowerTiltServo.attach(LOWER_TILT_SERVO_PIN);
    upperTiltServo.attach(UPPER_TILT_SERVO_PIN);
    gripServo.attach(GRIP_SERVO_PIN);
}

void writeServo(
    int yaw, int lowerTilt,
    int upperTilt, int grip
) {
    yawServo.write(yaw);
    lowerTiltServo.write(lowerTilt);
    upperTiltServo.write(upperTilt);
    gripServo.write(grip);
}

void setup() {
  initServo();
  writeServo(0, 0, 180, 0);
  delay(1000);
  // writeServo(0, 0, 160, 0);
  // writeServo(0, 20, 160, 0);

  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(joystick_btn_pin, INPUT_PULLUP);
  // pinMode(button_pin, INPUT_PULLUP);

  // attachInterrupt(digitalPinToInterrupt(button_pin), isr, FALLING);
  // attachInterrupt(digitalPinToInterrupt(joystick_btn_pin), isr_2, FALLING);
  Serial.begin(9600);
  
  pinMode(x_pin, INPUT);
  // pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

int current_servo = 1;

void loop() {
    const boolean enable_edit = true;
    int x_input {analogRead(A0)};
    int y_input {analogRead(A1)};

/*
     if (x_input < 300 && enable_edit) {
        if (servo_pos[current_servo] < 180) {
            servo_pos[current_servo] += 1; 
            delay(100);
        }
      } else if (x_input > 900 && enable_edit) {
        if (servo_pos[current_servo] > 0) {
            servo_pos[current_servo] -= 1;
            delay(100);
        }
      }
      
      if (y_input < 300 && enable_edit) {
        if (current_servo > 0) {
            current_servo = current_servo - 1;
        }
        
        Serial.print("SERVO DW ");
        Serial.println(current_servo);
        delay(1000);
        
      } else if (y_input > 900 && enable_edit) {
        if (current_servo < 3) {
            current_servo = current_servo + 1;
        }

        Serial.print("SERVO UP ");
        Serial.println(current_servo);
        delay(1000);
      }

    if (current_servo == 4) {
        current_servo = 3;
    }

    writeServo(servo_pos[0], servo_pos[1], servo_pos[2], servo_pos[3]);
    */
    int button_value = digitalRead(button2);
    

    Serial.print("Serial pos: ");
    Serial.print(servo_pos[0]);
    Serial.print(" ");
    Serial.print(servo_pos[1]);
    Serial.print(" ");
    Serial.print(servo_pos[2]);
    Serial.print(" ");
    Serial.print(servo_pos[3]);
    Serial.print(" ");
    Serial.print(current_servo);
    Serial.print(" ");
    Serial.print(button_value);
    Serial.println(" ");
    
}
