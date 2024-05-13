#ifndef OAUTH_H
#define OAUTH_H

#include "ArduinoJson.h"
#include "base64.h"
#include "secrets.h"

String getAuthorizeUrl();
String getTokenRequestPayload(String authCode);
void storeAccessToken(JsonDocument json);
String getBasicAuthorizationHeader();
String getBearerAuthorizationHeader();

#endif
