#include "wifi.h"
#include "http-server.h"
#include "http-handlers.h"

void setup() {
    Serial.begin(115200);
    Serial.println("IoT Record Player");

    ensureWifiConnection();
    startHttpServer();
}

void loop() {
    ensureWifiConnection();
    manageHttpClientConnections();
    handleHttpClientRequests(httpHandlers);
}