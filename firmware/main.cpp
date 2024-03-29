/*
 * Blinky Controller
 *
* Copyright (c) 2014 Matt Mets
 *
 * based on Fadecandy Firmware, Copyright (c) 2013 Micah Elizabeth Scott
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "WProgram.h"
#include "arm_math.h"
#include "usb_serial.h"
#include "usb_dev.h"

#include "digitalysplitter.h"
#include "dmx.h"
#include "patterns.h"
#include "serialloop.h"
#include "buttons.h"

// Button inputs
Buttons userButtons;

// Reserved RAM area for signalling entry to bootloader
extern uint32_t boot_token;

static void dfu_reboot()
{
    // Reboot to the Fadecandy Bootloader
    boot_token = 0x74624346;

    // Short delay to allow the host to receive the response to DFU_DETACH.
    uint32_t deadline = millis() + 10;
    while (millis() < deadline) {
        watchdog_refresh();
    }

    // Detach from USB, and use the watchdog to time out a 10ms USB disconnect.
    __disable_irq();
    USB0_CONTROL = 0;
    while (1);
}


void setupWatchdog() {
    // Change the watchdog timeout because the SPI access is too slow.
    const uint32_t watchdog_timeout = F_BUS / 2;  // 500ms

    WDOG_UNLOCK = WDOG_UNLOCK_SEQ1;
    WDOG_UNLOCK = WDOG_UNLOCK_SEQ2;
    asm volatile ("nop");
    asm volatile ("nop");
    WDOG_STCTRLH = WDOG_STCTRLH_ALLOWUPDATE | WDOG_STCTRLH_WDOGEN |
        WDOG_STCTRLH_WAITEN | WDOG_STCTRLH_STOPEN | WDOG_STCTRLH_CLKSRC;
    WDOG_PRESC = 0;
    WDOG_TOVALH = (watchdog_timeout >> 16) & 0xFFFF;
    WDOG_TOVALL = (watchdog_timeout)       & 0xFFFF;
}


// For board bringup
#define OUTPUT_COUNT 9
uint8_t outputs[OUTPUT_COUNT] = {
    LED_0_PIN,
    LED_1_PIN,
    LED_2_PIN,
    LED_3_PIN,
    LED_4_PIN,
    DATA_A_PIN,
    CLOCK_A_PIN,
    DATA_B_PIN,
    CLOCK_B_PIN,
//    DATA_IN_PIN,
//    CLOCK_IN_PIN
};

// For board bringup
void toggleOutput(uint8_t output) {
    for(int i = 0; i < OUTPUT_COUNT; i++) {
        digitalWrite(outputs[i], HIGH);
    }
    digitalWrite(outputs[output], LOW);
}

int currentOutput = 0;

extern "C" int main()
{
    setupWatchdog();

    initBoard();

    pinMode(DATA_IN_PIN, INPUT);
    pinMode(CLOCK_IN_PIN, INPUT);

    toggleOutput(currentOutput);

    userButtons.setup();

//    dmxSetup();

    serialReset();

    // Application main loop
    while (usb_dfu_state == DFU_appIDLE) {
        watchdog_refresh();
       
        // TODO: put this in an ISR? Make the buttons do pin change interrupts?
        userButtons.buttonTask();

        if(digitalRead(DATA_IN_PIN)) {
            digitalWrite(LED_0_PIN, HIGH);
        }
        else {
            digitalWrite(LED_0_PIN, LOW);
        }
        if(digitalRead(CLOCK_IN_PIN)) {
            digitalWrite(LED_1_PIN, HIGH);
        }
        else {
            digitalWrite(LED_1_PIN, LOW);
        }

        // If the flash wasn't initialized, show a default flashing pattern
//        count_up_loop();
//        dmxShow();

        // Check for serial data
        if(usb_serial_available() > 0) {
            serialLoop();
        }

        if(userButtons.isPressed()) {
            uint8_t button = userButtons.getPressed();
    
            if(button == BUTTON_A) {
                currentOutput = (currentOutput+1)%OUTPUT_COUNT;
                toggleOutput(currentOutput);
            }
            else if(button == BUTTON_B) {
                currentOutput = (currentOutput+OUTPUT_COUNT-1)%OUTPUT_COUNT;
                toggleOutput(currentOutput);
            }
        }

    }

    // Reboot into DFU bootloader
    dfu_reboot();
}
