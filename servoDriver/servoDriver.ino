#include <Servo.h>

Servo yawServo;
Servo lowerTiltServo;
Servo upperTiltServo;
Servo gripServo;

int servoSequence[4][SEQ_SIZE] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
int servoPosition[4] = {0,0,0,0};

void initServo();

void initServo() {
    yawServo.attach(YAW_SERVO_PIN);
    lowerTiltServo.attach(LOWER_TILT_SERVO_PIN);
    upperTiltServo.attach(UPPER_TILT_SERVO_PIN);
    gripServo.attach(GRIP_SERVO_PIN);
}

void writeServo(int yaw,
                int lowerTilt,
                int upperTilt,
                int grip) {
    yawServo.write(yaw);
    lowerTiltServo.write(lowerTilt);
    upperTiltServo.write(upperTilt);
    gripServo.write(grip);
}

void servoPlaySequence() {
    for (int i = 0; i < SEQ_SIZE; ++i) {
        writeServo(servoSequence[0][i],
                   servoSequence[1][i],
                   servoSequence[2][i],
                   servoSequence[3][i]);
        delay(SEQ_DELAY);
    }
}

void servoRecordSequence() {

}

void servoTestPosition() {

}

void servoSaveRecordedSequence() {

}

void servoClearSequenceArray() {

}

void servoSweepTest() {
    for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        writeServo(pos, pos, pos, pos);
        delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        writeServo(pos, pos, pos, pos);
        delay(15);                       // waits 15ms for the servo to reach the position
    }
}

void zeroAllServos() {
    
}