#include "nfc.h"
#include "wifi.h"
#include "http-server.h"
#include "http-handlers.h"
#include "http-client.h"
#include "player.h"

void setup() {
    Serial.begin(115200);
    Serial.println("IoT Record Player");

    initializeNfcReader();
    ensureWifiConnection();
    configureCertificates();
    startHttpServer();
}

void loop() {
    ensureWifiConnection();
    ensureValidAccessToken();
    manageHttpClientConnections();
    handleHttpClientRequests(httpHandlers);

    String data = readNfcData();
    if (data.length() > 0) {
        startPlayback(data);
        delay(5000);
    }
}
