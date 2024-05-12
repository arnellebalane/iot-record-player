#include "wifi.h"
#include "http-server.h"
#include "http-handlers.h"
#include "http-client.h"

void setup() {
    Serial.begin(115200);
    Serial.println("IoT Record Player");

    ensureWifiConnection();
    configureCertificates();
    startHttpServer();
}

void loop() {
    ensureWifiConnection();
    manageHttpClientConnections();
    handleHttpClientRequests(httpHandlers);
}
