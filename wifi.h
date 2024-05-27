#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "storage.h"
#include "secrets.h"

#define WIFI_MODE_STATION 0
#define WIFI_MODE_ACCESS_POINT 1

extern int wifiMode;

void determineWifiMode();
void ensureWifiConnection();
void initializeAccessPoint();
void initializeDns();

#endif
