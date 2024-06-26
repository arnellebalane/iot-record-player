#ifndef SECRETS_H
#define SECRETS_H

#include <Arduino.h>

extern String OAUTH_CLIENT_ID;
extern String OAUTH_CLIENT_SECRET;

extern String OAUTH_AUTHORIZE_URL;
extern String OAUTH_TOKEN_URL;
extern String OAUTH_REDIRECT_URL;
extern String OAUTH_SCOPES;

extern String PLAYER_PLAYBACK_URL;

extern String WIFI_AP_SSID;
extern String WIFI_AP_PASSWORD;
extern String WIFI_HOSTNAME;

extern uint8_t NFC_AUTHENTICATION_KEY[6];

// Root CA certificate for the URL's SSL certificate
// https://www.dzombak.com/blog/2021/10/HTTPS-Requests-with-a-Small-Set-of-Root-Certificates-on-ESP8266-Arduino.html
extern String CA_CERTIFICATE;

#endif
