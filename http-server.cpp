#include "WiFiServer.h"
#include "WiFiClient.h"
#include "http-request.h"
#include "http-handlers.h"

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

void handleHttpClientRequests(HttpHandlersMap handlers) {
    for (int i = 0; i < 8; i++) {
        while (clients[i] && clients[i].available()) {
            HttpRequest request = parseHttpRequest(clients[i]);

            HttpHandlersMap::iterator iterator = handlers.find(request.path);
            if (iterator == handlers.end()) {
                clients[i].println("HTTP/1.1 404 Not Found\n");
            } else {
                HttpHandler handler = handlers[request.path];
                handler(clients[i], request);
            }
        }
    }
}
