// based on
// https://learn.adafruit.com/matrix-keypad/pinouts
// https://github.com/adafruit/Adafruit_Keypad/blob/master/examples/keypad_test/keypad_test.ino

#include "Adafruit_Keypad.h"

#define R1 7
#define R2 8
#define R3 9
#define R4 10
#define C1 11
#define C2 12
#define C3 13

const byte ROWS = 4; // rows
const byte COLS = 3; // columns
// define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
    {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3};

// initialize an instance of class Keypad
Adafruit_Keypad customKeypad =
    Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int led_pin = 2;
const int buzzer_pin = 3;

// code length
const int input_length_max = 4;
// setup the buffer
String input_buffer = "";

const String correct_code = "1234";

void setup() {
    Serial.begin(115200);
    customKeypad.begin();
    pinMode(led_pin, OUTPUT);
    pinMode(buzzer_pin, OUTPUT);
    input_buffer_reset();
}

void loop() {
    customKeypad.tick();
    handle_keypad();
}

void input_buffer_reset() { input_buffer = ""; }

void handle_keypad() {
    while (customKeypad.available()) {
        keypadEvent e = customKeypad.read();
        if (e.bit.EVENT == KEY_JUST_PRESSED) {
            digitalWrite(buzzer_pin, HIGH);
        }
        if (e.bit.EVENT == KEY_JUST_RELEASED) {
            digitalWrite(buzzer_pin, LOW);
            char key = (char)e.bit.KEY;
            handle_key(key);
        }
    }
}

void handle_key(char key) {
    // new input. so we deactivate
    digitalWrite(led_pin, LOW);
    if (key == '*') {
        // if we press '*' we reset the input buffer.
        input_buffer_reset();
    } else if (input_buffer.length() < input_length_max) {
        input_buffer += key;
        if (input_buffer.length() >= input_length_max) {
            input_buffer_handle_ready();
            input_buffer_reset();
        }
    } else {
        input_buffer_reset();
    }
}

void input_buffer_handle_ready() {
    Serial.println(input_buffer);
    if (input_buffer == correct_code) {
        code_correct();
    } else {
        code_wrong();
    }
}

void code_correct() {
    digitalWrite(led_pin, HIGH);
    // correct code given :-)!
    delay(1000);
    digitalWrite(led_pin, LOW);
}

void code_wrong() {
    digitalWrite(led_pin, LOW);
    digitalWrite(buzzer_pin, HIGH);
    delay(100);
    digitalWrite(buzzer_pin, LOW);
    delay(100);
    digitalWrite(buzzer_pin, HIGH);
    delay(100);
    digitalWrite(buzzer_pin, LOW);
    delay(100);
    digitalWrite(buzzer_pin, HIGH);
    delay(100);
    digitalWrite(buzzer_pin, LOW);
    delay(100);
}