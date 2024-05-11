#include "WiFiServer.h"
#include "WiFiClient.h"

#include "http-request.h"

WiFiServer server(80);
WiFiClient clients[8];

void startHttpServer() {
    server.begin();
    Serial.println("HTTP server is now running");
}

void manageHttpClientConnections() {
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
            HttpRequest request = parseHttpRequest(clients[i]);

            Serial.println(request.method + " " + request.path);
            Serial.println(request.search["code"]);
        }
    }
}
