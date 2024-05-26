#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include "secrets.h"

#define WIFI_MODE_STATION 0
#define WIFI_MODE_ACCESS_POINT 1

extern int wifiMode;

void determineWifiMode();
void ensureWifiConnection();

#endif
