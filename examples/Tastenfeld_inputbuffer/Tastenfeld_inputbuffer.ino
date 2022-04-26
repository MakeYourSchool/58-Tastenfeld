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





#include <Wire.h>
#include <rgb_lcd.h>
rgb_lcd lcd;







// here you define how many characters are allowed for the input
const int input_lengt_max = 3;

// setup the buffer
char input_buffer[input_lengt_max+1];

// input handling
int input_buffer_position = 0;

// this flag will is true if the input_buffer is full / we ended the input.
bool input_ready = false;





void setup() {
    Serial.begin(115200);
    customKeypad.begin();

    lcd.begin(16, 2);
    lcd.setRGB(0, 230, 255);
    lcd.setCursor(6, 0);
    lcd.print("Hallo");

    input_buffer_reset();
}





void loop() {
    customKeypad.tick();
    handle_keypad();
    delay(10);
}









void input_buffer_reset() {
    // clear input buffer
    // for every cell in the buffer set it to '\0' (= that is the *string end symbol*)
    for (size_t i = 0; i < sizeof(input_buffer); i++) {
        input_buffer[i] = '\0';
    }
    // the following line does the same ;-)
    // it is maybe a little bit faster... but also maybe harder to understand..
    // memset(input_buffer, '\0', sizeof(input_buffer));

    // reset our pointers..
    input_buffer_position = 0;
    input_ready = false;
}


void handle_keypad() {
    while (customKeypad.available()) {
        keypadEvent e = customKeypad.read();
        if (e.bit.EVENT == KEY_JUST_RELEASED) {
            char key = (char)e.bit.KEY;
            handle_key(key);
        }
    }
}

void handle_key(char key) {
    if (key == '*') {
        // if we press '*' we reset the input buffer.
        input_buffer_reset();
    } else if (key == '#') {
        // if we press '#' we know that the input is doen.
        input_ready = true;
    }

    if (!input_ready) {
        input_buffer_position = input_buffer_position + 1;
        if (input_buffer_position < sizeof(input_buffer) ) {
            input_buffer[input_buffer_position] = key;
        } else {
            input_ready = true;
        }

    }

    if (input_ready) {
        input_buffer_handle_ready();
        input_buffer_reset();
    }
}



void input_buffer_handle_ready() {
    Serial.println(input_buffer);
    lcd.setCursor(6, 1);
    lcd.print(input_buffer);

    // if we wnat to convert the string to an integer (=number)
    // https://www.arduino.cc/en/Tutorial/BuiltInExamples/StringToInt
    // int input_as_number = String(input_buffer).toInt();
    // Serial.println(input_as_number);
    
    // same but a little bit easier / faster
    int input_as_number = atoi(input_buffer);
    Serial.println(input_as_number);

}
