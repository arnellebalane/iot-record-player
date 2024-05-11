#include <functional>
#include <map>
#include "WiFiClient.h"
#include "oauth.h"
#include "http-request.h"
#include "http-handlers.h"

void startAuthorization(WiFiClient* client, HttpRequest* request) {
    Serial.println("Starting authorization");
    String authorizeUrl = getAuthorizeUrl();
    (*client).println("HTTP/1.1 302 Found");
    (*client).println("Location: " + authorizeUrl);
    (*client).println();
}

void receiveAuthorizationCode(WiFiClient* client, HttpRequest* request) {
    Serial.println("Received authorization code");
}

HttpHandlersMap httpHandlers = {
    {"/", startAuthorization},
    {"/oauth/spotify", receiveAuthorizationCode}
};
