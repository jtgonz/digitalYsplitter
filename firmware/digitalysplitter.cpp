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


    // TODO: Use pinmode for these

    // LED 0
    PORTA_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;
    GPIOA_PDDR |= (1<<1);

    // LED 1
    PORTA_PCR2 = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;
    GPIOA_PDDR |= (1<<2);

    // LED 2
    PORTA_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;
    GPIOA_PDDR |= (1<<4);

    // LED 3
    PORTB_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;
    GPIOB_PDDR |= (1<<0);

    // LED 4
    PORTB_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;
    GPIOB_PDDR |= (1<<1);


//    setStatusLed(LED_0_PIN, 255);
//    setStatusLed(LED_1_PIN, 255);
//    setStatusLed(LED_2_PIN, 255);
//    setStatusLed(LED_3_PIN, 255);
//    setStatusLed(LED_4_PIN, 255);
}

void setStatusLed(uint8_t pin, uint8_t value) {
    analogWrite(pin, (255-value));
}
