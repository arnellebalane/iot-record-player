#include <ESP8266WiFi.h>
#include "storage.h"
#include "secrets.h"
#include "wifi.h"

#define WIFI_MODE_PIN 5 // D1

int wifiMode;

void determineWifiMode() {
    if (digitalRead(WIFI_MODE_PIN) == LOW) {
        wifiMode = WIFI_MODE_STATION;
    } else {
        wifiMode = WIFI_MODE_ACCESS_POINT;
    }
}

void ensureWifiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to network");
        String ssid = readWifiSsid();
        String password = readWifiPassword();

        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Connected to network");

        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void initializeAccessPoint() {
    Serial.println("Initializing access point");
    if (WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD)) {
        Serial.println("Access point initialized successfully");
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("Failed to initialize access point");
        while (1);
    }
}
