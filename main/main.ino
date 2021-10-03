#include "servoDriver.h"

char receivedByte = 0;

void actuate(char receivedByte) {
    bool validInput = true;
    switch (receivedByte) {
        case 'P':
            servoPlaySequence();
            Serial.println("play sequence");
            break;
        case 'R':
            servoRecordSequence();
            Serial.println("record sequence");
            break;
        case 'T':
            servoTestPosition();
            Serial.println("test sequence");
            break;
        case 'S':
            servoSaveRecordedSequence();
            Serial.println("save sequence");
            break;
        case 'C':
            servoClearSequenceArray();
            Serial.println("clear sequence");
            break;
        default:
            // Serial.println("Invalid command");
            validInput = false;
    }
    if (validInput) {
    }
}

void listenAndActuate() {
    if (Serial.available() > 0) {
        receivedByte = Serial.read();
        actuate(receivedByte);
    }
}

void setup() {
    Serial.begin(115200);
    initServo();
}

void loop() {
    listenAndActuate();
}