#ifndef OAUTH_H
#define OAUTH_H

#include "secrets.h"

String getAuthorizeUrl();
String getTokenRequestPayload(String authCode);
String getBasicAuthorizationHeader();

#endif
