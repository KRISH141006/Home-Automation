#include <Arduino.h>

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "relay_manager.h"
#include "switch_manager.h"
#include "webserver_manager.h"

void setup() {

    Serial.begin(115200);

    Serial.println();
    Serial.println("=================================");
    Serial.println("SMARTGRUH PROTOTYPE - SINGLE SWITCH");
    Serial.println("=================================");

    // Relays
    setupRelays();

    // Switches
    setupSwitches();

    // WiFi with AP Fallback
    setupWiFi();

    // MQTT
    setupMQTT();

    // Local Web Server
    setupWebServer();

    Serial.println("=================================");
    Serial.println("SYSTEM READY");
    Serial.println("=================================");
}

void loop() {

    mqttLoop();

    handleSwitches();

    handleWebServer();

    // Debug State Every 5s
    static unsigned long lastDebug = 0;
    if(millis() - lastDebug > 5000) {

        lastDebug = millis();

        Serial.print("[STATE] Bulb: ");
        Serial.println(getRelayState() ? "ON" : "OFF");
    }
}
