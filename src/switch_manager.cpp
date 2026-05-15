#include <Arduino.h>

#include "switch_manager.h"
#include "config.h"
#include "relay_manager.h"
#include "mqtt_manager.h"

volatile bool switchPressed = false;

unsigned long lastInterrupt = 0;

void IRAM_ATTR switchISR()
{
    unsigned long now = millis();

    if(now - lastInterrupt > 250)
    {
        switchPressed = true;
        lastInterrupt = now;
    }
}

void setupSwitches()
{
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(SWITCH_PIN),
        switchISR,
        CHANGE
    );
}

void handleSwitches()
{
    if(switchPressed)
    {
        switchPressed = false;

        toggleRelay();

        publishState(
            TOPIC_STATUS,
            getRelayState()
        );
    }
}