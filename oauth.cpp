#include <ArduinoJson.h>
#include <base64.h>
#include "http-client.h"
#include "storage.h"
#include "secrets.h"
#include "oauth.h"

base64 b64;

String accessToken = "";
String refreshToken = "";
int tokenValidity;
double tokenIssueTime;

String getAuthorizeUrl() {
    return OAUTH_AUTHORIZE_URL
        + "?client_id=" + OAUTH_CLIENT_ID
        + "&response_type=code"
        + "&redirect_uri=" + OAUTH_REDIRECT_URL
        + "&scope=" + OAUTH_SCOPES;
}

String getTokenRequestPayload(String authCode) {
    return "client_id=" + OAUTH_CLIENT_ID
        + "&grant_type=authorization_code"
        + "&code=" + authCode
        + "&redirect_uri=" + OAUTH_REDIRECT_URL;
}

String getTokenRefreshPayload() {
    return "client_id=" + OAUTH_CLIENT_ID
        + "&grant_type=refresh_token"
        + "&refresh_token=" + refreshToken;
}

void storeAccessToken(JsonDocument json) {
    accessToken = json["access_token"].as<String>();
    tokenValidity = json["expires_in"].as<int>();
    tokenIssueTime = millis() / 1000.0;
    if (json.containsKey("refresh_token")) {
        refreshToken = json["refresh_token"].as<String>();
        storeRefreshToken(refreshToken);
    }
}

void initializeAccessToken() {
    refreshToken = readRefreshToken();
    if (refreshToken.length() > 0) {
        refreshAccessToken();
    }
}

void ensureValidAccessToken() {
    if (tokenValidity > 0) {
        double now = millis() / 1000.0;
        if (now - tokenIssueTime >= tokenValidity) {
            Serial.println("Access token has expired");
            if (refreshToken == "") {
                Serial.println("Refresh token not found, unable to refresh access token");
                return;
            }
            refreshAccessToken();
        }
    }
}

void refreshAccessToken() {
    Serial.println("Refreshing access token");

    String payload = getTokenRefreshPayload();
    HttpResponse response = sendHttpRequest("POST", OAUTH_TOKEN_URL, payload, {
        {"Authorization", getBasicAuthorizationHeader()},
        {"Content-Type", "application/x-www-form-urlencoded"}
    });

    if (response.status == HTTP_CODE_OK) {
        Serial.println("Received new access token");

        JsonDocument json;
        DeserializationError jsonError = deserializeJson(json, response.body);
        if (jsonError) {
            Serial.println("Failed to deserialize access token");
        } else {
            storeAccessToken(json);
        }
    } else {
        Serial.println("Failed to refresh access token");
    }
}

String getBasicAuthorizationHeader() {
    String original = OAUTH_CLIENT_ID + ":" + OAUTH_CLIENT_SECRET;
    String encoded = b64.encode(original);
    return "Basic " + encoded;
}

String getBearerAuthorizationHeader() {
    return "Bearer " + accessToken;
}
