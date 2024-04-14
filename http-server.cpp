#include "WiFiServer.h"
#include "WiFiClient.h"

WiFiServer server(80);
WiFiClient clients[8];

void startHttpServer() {
    server.begin();
    Serial.println("HTTP server is now running");
}

void maintainHttpClientConnections() {
    WiFiClient client = server.accept();
    if (client) {
        for (int i = 0; i < 8; i++) {
            if (!clients[i] || !clients[i].connected()) {
                Serial.print("New client connection: ");
                Serial.println(client.remoteIP());

                clients[i] = client;
                break;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        if (clients[i] && !clients[i].connected()) {
            clients[i].stop();
        }
    }
}

void handleHttpClientRequests() {
    for (int i = 0; i < 8; i++) {
        while (clients[i] && clients[i].available()) {
            String request = clients[i].readStringUntil('\n');

            // Ignore the rest of the request
            String rest;
            do {
                rest = clients[i].readStringUntil('\n');
            } while (rest.length() > 0);

            String parts[3];
            int i = 0;
            int index = request.indexOf(' ');
            while (index >= 0 && i < 3) {
                parts[i++] = request.substring(0, index);
                request = request.substring(index + 1);
                index = request.indexOf(' ');
            }

            Serial.println("Method: " + parts[0]);
            Serial.println("Path:   " + parts[1]);
        }
    }
}
