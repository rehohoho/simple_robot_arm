#pragma once

const int YAW_SERVO_PIN = 6;
const int LOWER_TILT_SERVO_PIN = 7;
const int UPPER_TILT_SERVO_PIN = 8;
const int GRIP_SERVO_PIN = 9;

const int SEQ_DELAY = 1000;
const int SEQ_SIZE = 4;

void initServo();
void writeServo(int, int, int, int);
void writePosition(int, int);

void servoPlaySequence();
void servoRecordSequence();
void servoTestPosition();
void servoSaveRecordedSequence();
void servoClearSequenceArray();

void servoSweepTest();
void zeroAllServos();
void maxAllServos();

void releaseUpTurnLeft();
void upTurnRight();