#include "ESP8266WiFi.h"
#include "secrets.h"

void ensureWifiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to network");
        WiFi.begin(secrets::WIFI_SSID, secrets::WIFI_PASSWORD);
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
