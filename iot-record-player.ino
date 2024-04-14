#include "wifi.h"
#include "oauth.h"

void setup() {
    Serial.begin(115200);
    Serial.println("IoT Record Player");
}

void loop() {
    ensureWifiConnection();

    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() > 0) {
        Serial.println(message);
        if (message == "start_oauth") {
            String authorizeUrl = getAuthorizeUrl();
            Serial.print("Authorize URL: ");
            Serial.println(authorizeUrl);
        }
    }
}
