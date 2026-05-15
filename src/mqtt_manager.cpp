#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#include "mqtt_manager.h"
#include "config.h"
#include "relay_manager.h"

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

void mqttCallback(
    char* topic,
    byte* payload,
    unsigned int length
) {

    String message;

    for(int i = 0; i < length; i++) {

        message += (char)payload[i];
    }

    Serial.print("[MQTT] Received: ");
    Serial.print(topic);
    Serial.print(" -> ");
    Serial.println(message);

    bool state = (message == "ON");

    if(String(topic) == TOPIC_STATUS) {
        setRelay(state);
    }
}

void setupMQTT() {

    espClient.setInsecure();

    mqttClient.setServer(
        MQTT_BROKER,
        MQTT_PORT
    );

    mqttClient.setCallback(mqttCallback);
}

void reconnectMQTT() {

    // Only attempt MQTT if WiFi is connected
    if(WiFi.status() != WL_CONNECTED) return;

    static unsigned long lastReconnect = 0;

    if(millis() - lastReconnect > 5000) {

        lastReconnect = millis();

        Serial.println("[MQTT] Connecting...");

        if(
            mqttClient.connect(
                MQTT_CLIENT_ID,
                MQTT_USERNAME,
                MQTT_PASSWORD
            )
        ) {

            Serial.println("[MQTT] Connected");

            mqttClient.subscribe(TOPIC_STATUS);

            // Publish initial state on connection
            publishState(
                TOPIC_STATUS,
                getRelayState()
            );
        }

        else {

            Serial.print("[MQTT] Failed, rc = ");
            Serial.println(mqttClient.state());
        }
    }
}

void mqttLoop() {

    if(!mqttClient.connected()) {

        reconnectMQTT();
    }

    mqttClient.loop();
}

void publishState(
    const char* topic,
    bool state
) {
    if(mqttClient.connected()) {
        mqttClient.publish(
            topic,
            state ? "ON" : "OFF"
        );
    }
}
