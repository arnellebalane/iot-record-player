#ifndef OAUTH_H
#define OAUTH_H

#include <ArduinoJson.h>
#include <base64.h>
#include "http-client.h"
#include "storage.h"
#include "secrets.h"

String getAuthorizeUrl();
String getTokenRequestPayload(String authCode);
String getTokenRefreshPayload();
void storeAccessToken(JsonDocument json);
void initializeAccessToken();
void ensureValidAccessToken();
void refreshAccessToken();
String getBasicAuthorizationHeader();
String getBearerAuthorizationHeader();

#endif
