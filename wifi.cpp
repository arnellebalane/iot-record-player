#include <ESP8266WiFi.h>
#include "secrets.h"
#include "wifi.h"

int wifiMode;

void determineWifiMode() {
    wifiMode = WIFI_MODE_STATION;
}

void ensureWifiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to network");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
