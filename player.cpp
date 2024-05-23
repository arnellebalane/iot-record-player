#include "http-client.h"
#include "oauth.h"
#include "secrets.h"

void startPlayback(String uri) {
    Serial.println("Starting playback: " + uri);

    String payload = "{\"context_uri\":\"" + uri + "\",\"position\":0}";
    sendHttpRequest("PUT", PLAYER_PLAYBACK_URL, payload, {
        {"Authorization", getBearerAuthorizationHeader()}
    });
}
