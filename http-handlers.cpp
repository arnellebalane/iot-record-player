#include <functional>
#include <map>
#include "WiFiClient.h"
#include "secrets.h"
#include "oauth.h"
#include "http-request.h"
#include "http-handlers.h"
#include "http-client.h"

void startAuthorization(WiFiClient* client, HttpRequest* request) {
    Serial.println("Starting authorization");

    String authorizeUrl = getAuthorizeUrl();
    (*client).println("HTTP/1.1 302 Found");
    (*client).println("Location: " + authorizeUrl);
    (*client).println();
}

void receiveAuthorizationCode(WiFiClient* client, HttpRequest* request) {
    Serial.println("Received redirect from authorization server");
    SearchParams::iterator iterator = request->search.find("code");
    if (iterator == request->search.end()) {
        Serial.println("Authorization code not received");
    } else {
        Serial.println("Received authorization code");
        Serial.println("Retrieving access token");

        String authCode = request->search["code"];
        String payload = getTokenRequestPayload(authCode);
        String authorizationHeader = getBasicAuthorizationHeader();
        HttpResponse response = sendHttpRequest("POST", OAUTH_TOKEN_URL, payload, {
            {"Authorization", authorizationHeader},
            {"Content-Type", "application/x-www-form-urlencoded"}
        });

        if (response.status == HTTP_CODE_OK) {
            Serial.println("Received access token");
            Serial.println(response.body);
            client.println("HTTP/1.1 200 OK\n");
        } else {
            Serial.println("Failed to retrieve access token");
            Serial.println("Status code: " + response.status);
            client.println("HTTP/1.1 400 Bad Request\n");
        }
    }
}

HttpHandlersMap httpHandlers = {
    {"/", startAuthorization},
    {"/oauth/spotify", receiveAuthorizationCode}
};
