#include "servoDriver.h"

char receivedByte = 0;

String inputString = "";
bool stringComplete;


void serialEvent() {
    while (Serial.available()) {
        char inChar = (char) Serial.read();
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}

void manualInput(String inputString) {
    
    int inputStrLen = inputString.length();
    char inputChars[inputStrLen];
    inputString.toCharArray(inputChars, inputStrLen);
    char* command = strtok(inputChars, ",");
    
    int steerAngles[inputStrLen];
    int i = 0;

    while (command != NULL) {
        steerAngles[i] = atoi(command);
        Serial.print("Angle");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(steerAngles[i]);
        command = strtok(NULL, ",");
        i++;
    }
    
    writeServo(steerAngles[0], steerAngles[1], steerAngles[2], steerAngles[3]);
}

void setup() {
    Serial.begin(115200);
    initServo();
    inputString.reserve(200);
}

void loop() {
    if (stringComplete) {
        Serial.println(inputString);
        manualInput(inputString);
        inputString = "";
        stringComplete = false;
    }

    // zeroAllServos();
    // servoSweepTest();
    // servoTestPosition();
}