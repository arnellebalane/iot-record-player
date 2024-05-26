#include <ESP8266WebServer.h>
#include "oauth.h"
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

void handleRoot() {
    Serial.println(server.method());
    Serial.println(server.uri());
    Serial.println(server.arg("code"));
    Serial.println(server.arg("plain"));
    server.send(200, "text/plain", "OK");
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
        } else {
            storeAccessToken(json);
            server.send(200, "text/plain", "OK");
        }
    } else {
        Serial.println("Failed to retrieve access token");
        server.send(500, "text/plain", "ERROR");
    }
}

HttpHandlersMap httpHandlersStation = {
    {"/", startAuthorization},
    {"/oauth/spotify", receiveAuthorizationCode}
};
