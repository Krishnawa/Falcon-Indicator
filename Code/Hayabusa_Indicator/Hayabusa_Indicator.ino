/*
 * ATTINY13A DRL Controller
 *
 * INPUTS (Active LOW)
 * PB0 - Left Indicator
 * PB1 - Right Indicator
 *
 * OUTPUTS
 * PB4 - Left DRL
 * PB3 - Right DRL
 */

#include <Arduino.h>
#include <avr/io.h>

#define LEFT_IND      PB1
#define RIGHT_IND     PB2

#define DRL_RIGHT     PB3
#define DRL_LEFT      PB4

const uint16_t OFF_TIMEOUT = 1500;   // ms

uint32_t leftLastPulse = 0;
uint32_t rightLastPulse = 0;

void setup()
{
    // Inputs
    DDRB &= ~((1 << LEFT_IND) | (1 << RIGHT_IND));

    // Enable internal pullups if needed
    // PORTB |= (1 << LEFT_IND) | (1 << RIGHT_IND);

    // Outputs
    DDRB |= (1 << DRL_LEFT) | (1 << DRL_RIGHT);

    // DRLs ON at startup
    PORTB |= (1 << DRL_LEFT) | (1 << DRL_RIGHT);

    leftLastPulse = millis();
    rightLastPulse = millis();
}

void loop()
{
    uint32_t now = millis();
    uint8_t pins = PINB;

    bool leftIndicatorOn  = !(pins & (1 << LEFT_IND));   // Active LOW
    bool rightIndicatorOn = !(pins & (1 << RIGHT_IND));  // Active LOW

    // LEFT SIDE
    if (leftIndicatorOn)
    {
        leftLastPulse = now;

        // DRL OFF
        PORTB &= ~(1 << DRL_LEFT);
    }
    else
    {
        if ((now - leftLastPulse) >= OFF_TIMEOUT)
        {
            // DRL ON
            PORTB |= (1 << DRL_LEFT);
        }
    }

    // RIGHT SIDE
    if (rightIndicatorOn)
    {
        rightLastPulse = now;

        // DRL OFF
        PORTB &= ~(1 << DRL_RIGHT);
    }
    else
    {
        if ((now - rightLastPulse) >= OFF_TIMEOUT)
        {
            // DRL ON
            PORTB |= (1 << DRL_RIGHT);
        }
    }
}
