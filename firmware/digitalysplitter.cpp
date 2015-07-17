#include "WProgram.h"
#include "pins_arduino.h"
#include "digitalysplitter.h"

void initBoard() {
    pinMode(BUTTON_A_PIN, INPUT);
    pinMode(BUTTON_B_PIN, INPUT);

    pinMode(DATA_A_PIN, OUTPUT);
    pinMode(CLOCK_A_PIN, OUTPUT);
    pinMode(DATA_B_PIN, OUTPUT);
    pinMode(CLOCK_B_PIN, OUTPUT);

    
}

void setStatusLeds(uint8_t LED, uint8_t value) {
//    analogWrite(STATUS_LED_PIN, (255-value));
}
