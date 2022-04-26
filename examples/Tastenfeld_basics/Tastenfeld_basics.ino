// based on
// https://learn.adafruit.com/matrix-keypad/pinouts
// https://github.com/adafruit/Adafruit_Keypad/blob/master/examples/keypad_test/keypad_test.ino

#include "Adafruit_Keypad.h"

#define R1    2
#define R2    3
#define R3    4
#define R4    5
#define C1    6
#define C2    7
#define C3    8


const byte ROWS = 4; // rows
const byte COLS = 3; // columns
// define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3};


//initialize an instance of class Keypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);



void setup() {
    Serial.begin(115200);
    customKeypad.begin();
}





void loop() {
    customKeypad.tick();
    handle_keypad();
    delay(10);
}



void handle_keypad() {
    while (customKeypad.available()) {
        keypadEvent e = customKeypad.read();
        if (e.bit.EVENT == KEY_JUST_RELEASED) {
            char key = (char)e.bit.KEY;

            Serial.println(key);

        }
    }
}
