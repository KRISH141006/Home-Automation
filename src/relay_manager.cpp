#include <Arduino.h>

#include "relay_manager.h"
#include "config.h"

bool relayState = false;

void setupRelays() {
    // Write state BEFORE pinMode to avoid boot-up flicker
    digitalWrite(RELAY_PIN, LOW);
    pinMode(RELAY_PIN, OUTPUT);
    relayState = false;
}

void setRelay(bool state) {
    relayState = state;
    // Active High Logic: ON = HIGH, OFF = LOW
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
}

void toggleRelay() {
    setRelay(!relayState);
}

bool getRelayState() {
    return relayState;
}
