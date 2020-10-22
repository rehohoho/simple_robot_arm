#include "servoDriver.h"

char receivedByte = 0;

void actuate(char receivedByte) {
    bool validInput = true;
    switch (receivedByte) {
        case 'P':
            servoPlaySequence();
        case 'R':
            servoRecordSequence();
        case 'T':
            servoTestPosition();
        case 'S':
            servoSaveRecordedSequence();
        case 'C':
            servoClearSequenceArray();
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