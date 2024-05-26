#ifndef OAUTH_H
#define OAUTH_H

#include <ArduinoJson.h>
#include <base64.h>
#include "http-client.h"
#include "secrets.h"

String getAuthorizeUrl();
String getTokenRequestPayload(String authCode);
String getTokenRefreshPayload();
void storeAccessToken(JsonDocument json);
void ensureValidAccessToken();
String getBasicAuthorizationHeader();
String getBearerAuthorizationHeader();

#endif
