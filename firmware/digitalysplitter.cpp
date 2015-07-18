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

    pinMode(LED_0_PIN, OUTPUT);
    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
    pinMode(LED_4_PIN, OUTPUT);

    // TODO: delete me!
    pinMode(DATA_IN_PIN, OUTPUT);
    pinMode(CLOCK_IN_PIN, OUTPUT);
}

void setStatusLed(uint8_t pin, uint8_t value) {
    analogWrite(pin, (255-value));
}
