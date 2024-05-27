#include "nfc.h"
#include "wifi.h"
#include "http-server.h"
#include "http-client.h"
#include "player.h"

void setup() {
    Serial.begin(115200);
    Serial.println("IoT Record Player");

    determineWifiMode();

    if (wifiMode == WIFI_MODE_STATION) {
        initializeNfcReader();
        ensureWifiConnection();
        configureCertificates();
        startHttpServer();
        setHttpHandlers(httpHandlersStation);
    } else if (wifiMode == WIFI_MODE_ACCESS_POINT) {
        initializeAccessPoint();
        startHttpServer();
        setHttpHandlers(httpHandlersAccessPoint);
    }
}

void loop() {
    if (wifiMode == WIFI_MODE_STATION) {
        ensureWifiConnection();
        ensureValidAccessToken();
        handleHttpClients();

        String data = readNfcData();
        if (data.length() > 0) {
            startPlayback(data);
            delay(5000);
        }
    } else if (wifiMode == WIFI_MODE_ACCESS_POINT) {
        handleHttpClients();
    }
}
