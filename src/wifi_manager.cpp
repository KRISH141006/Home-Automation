#include <WiFi.h>
#include "wifi_manager.h"
#include "config.h"

bool isAPMode = false;

void setupWiFi() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("[WIFI] Connecting to ");
    Serial.println(WIFI_SSID);

    unsigned long startAttemptTime = millis();

    // Try connecting for 10 seconds
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("\n[WIFI] Connected");
        Serial.print("[WIFI] IP: ");
        Serial.println(WiFi.localIP());

        isAPMode = false;
    }

    else {

        Serial.println("\n[WIFI] Failed to connect. Starting AP...");

        WiFi.mode(WIFI_AP);
        WiFi.softAP(AP_SSID, AP_PASSWORD);

        Serial.print("[WIFI] AP Started: ");
        Serial.println(AP_SSID);
        Serial.print("[WIFI] AP IP: ");
        Serial.println(WiFi.softAPIP());

        isAPMode = true;
    }
}

bool checkWiFi() {

    if(!isAPMode && WiFi.status() != WL_CONNECTED) {

        // Try to reconnect or fallback to AP
        setupWiFi();
    }

    return (WiFi.status() == WL_CONNECTED);
}
