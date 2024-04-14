#ifndef SECRETS_H
#define SECRETS_H

#include "Arduino.h"

namespace secrets {
    extern const String OAUTH_CLIENT_ID;
    extern const String OAUTH_CLIENT_SECRET;

    extern const String OAUTH_AUTHORIZE_URL;
    extern const String OAUTH_TOKEN_URL;
    extern const String OAUTH_REDIRECT_URL;
    extern const String OAUTH_SCOPES;

    extern const String WIFI_SSID;
    extern const String WIFI_PASSWORD;
}

#endif
