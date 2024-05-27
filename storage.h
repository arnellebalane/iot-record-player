#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

void initializeStorage();
int getStorageAddress(int address);
void storeWifiCredentials(String ssid, String password);

#endif
