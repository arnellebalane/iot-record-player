#include <ESP8266WebServer.h>
#include "oauth.h"
#include "nfc.h"
#include "storage.h"
#include "secrets.h"
#include "http-client.h"
#include "http-server.h"

ESP8266WebServer server(80);

void startHttpServer() {
    server.begin();
    Serial.println("HTTP server is now running");
}

void handleHttpClients() {
    server.handleClient();
}

void setHttpHandlers(HttpHandlersMap handlers) {
    for (auto const &[path, handler] : handlers) {
        server.on(path, handler);
    }
}

// HTTP Request Handlers

void startAuthorization() {
    Serial.println("Starting authorization");

    String authorizeUrl = getAuthorizeUrl();
    server.sendHeader("Location", authorizeUrl);
    server.send(302);
}

void receiveAuthorizationCode() {
    Serial.println("Received redirect from authorization server");

    String authCode = server.arg("code");
    if (!authCode) {
        Serial.println("Authorization code not received");
        return;
    }

    Serial.println("Received authorization code");
    Serial.println("Retrieving access token");

    String payload = getTokenRequestPayload(authCode);
    HttpResponse response = sendHttpRequest("POST", OAUTH_TOKEN_URL, payload, {
        {"Authorization", getBasicAuthorizationHeader()},
        {"Content-Type", "application/x-www-form-urlencoded"}
    });

    if (response.status == HTTP_CODE_OK) {
        Serial.println("Received access token");

        JsonDocument json;
        DeserializationError jsonError = deserializeJson(json, response.body);
        if (jsonError) {
            Serial.println("Failed to deserialize access token");
            server.send(500, "text/plain", "ERROR");
            return;
        }

        storeAccessToken(json);
        server.send(200, "text/plain", "OK");
    } else {
        Serial.println("Failed to retrieve access token");
        server.send(500, "text/plain", "ERROR");
    }
}

void receiveWifiCredentials() {
    String body = server.arg("plain");
    body.trim();

    if (body.length() == 0) {
        Serial.println("Request body not found");
        server.send(400, "text/plain", "ERROR");
        return;
    }

    JsonDocument json;
    DeserializationError jsonError = deserializeJson(json, body);
    if (jsonError) {
        Serial.println("Failed to deserialize WiFi credentials");
        server.send(500, "text/plain", "ERROR");
        return;
    }
    if (!json.containsKey("ssid")) {
        Serial.println("'ssid' property not found");
        server.send(400, "text/plain", "ERROR");
        return;
    }

    String ssid = json["ssid"].as<String>();
    String password = "";
    if (json.containsKey("password")) {
        password = json["password"].as<String>();
    }

    storeWifiCredentials(ssid, password);
    server.send(200, "text/plain", "OK");
}

void writeContextToNfcCard() {
    String body = server.arg("plain");
    body.trim();

    if (body.length() == 0) {
        Serial.println("Request body not found");
        server.send(400, "text/plain", "ERROR");
        return;
    }

    JsonDocument json;
    DeserializationError jsonError = deserializeJson(json, body);
    if (jsonError) {
        Serial.println("Failed to deserialize context data");
        server.send(500, "text/plain", "ERROR");
        return;
    }
    if (!json.containsKey("context_uri")) {
        Serial.println("'context_uri' property not found");
        server.send(400, "text/plain", "ERROR");
        return;
    }

    String context = json["context_uri"].as<String>();
    Serial.println("Writing context to NFC card: " + context);

    while (!writeNfcData(context));
    server.send(200, "text/plain", "OK");
}

HttpHandlersMap httpHandlersStation = {
    {"/", startAuthorization},
    {"/oauth/spotify", receiveAuthorizationCode}
};

HttpHandlersMap httpHandlersAccessPoint = {
    {"/", receiveWifiCredentials},
    {"/write", writeContextToNfcCard}
};
