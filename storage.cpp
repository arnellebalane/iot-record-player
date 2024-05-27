#include <Arduino.h>
#include <EEPROM.h>

int storageCapacity = 512;

// There's usually some data stored in the first few bytes of the EEPROM (such
// as device UUID) not sure what they're for but skipping the first 64 bytes
// to avoid overwriting those pre-existing data.
int storageAddressOffset = 64;

void initializeStorage() {
    EEPROM.begin(storageCapacity);
}

int getStorageAddress(int address) {
    return address + storageAddressOffset;
}

void storeWifiCredentials(String ssid, String password) {
    int ssidLength = ssid.length();
    int passwordLength = password.length();

    // Use first three bytes of our memory space to store the lenths of WiFi
    // credentials and the refresh token, followed by the actual values themselves
    EEPROM.write(getStorageAddress(0), ssidLength);
    EEPROM.write(getStorageAddress(1), passwordLength);
    EEPROM.write(getStorageAddress(2), 0);

    int ssidOffset = 3;
    for (int i = 0; i < ssidLength; i++) {
        EEPROM.write(getStorageAddress(ssidOffset + i), (uint8_t) ssid.charAt(i));
    }

    int passwordOffset = 3 + ssidLength;
    for (int i = 0; i < passwordLength; i++) {
        EEPROM.write(getStorageAddress(passwordOffset + i), (uint8_t) password.charAt(i));
    }
    EEPROM.commit();

    Serial.println("WiFi credentials stored successfully");
}

void storeRefreshToken(String token) {
    int ssidLength = EEPROM.read(getStorageAddress(0));
    int passwordLength = EEPROM.read(getStorageAddress(1));
    int tokenLength = token.length();
    EEPROM.write(getStorageAddress(2), tokenLength);

    int tokenOffset = 3 + ssidLength + passwordLength;
    for (int i = 0; i < tokenLength; i++) {
        EEPROM.write(getStorageAddress(tokenOffset + i), (uint8_t) token.charAt(i));
    }
    EEPROM.commit();

    Serial.println("Refresh token stored successfully");
}

String readWifiSsid() {
    int ssidLength = EEPROM.read(getStorageAddress(0));
    int ssidOffset = 3;
    String ssid = "";
    for (int i = 0; i < ssidLength; i++) {
        ssid += (char) EEPROM.read(getStorageAddress(ssidOffset + i));
    }
    return ssid;
}

String readWifiPassword() {
    int ssidLength = EEPROM.read(getStorageAddress(0));
    int passwordLength = EEPROM.read(getStorageAddress(1));
    int passwordOffset = 3 + ssidLength;
    String password = "";
    for (int i = 0; i < passwordLength; i++) {
        password += (char) EEPROM.read(getStorageAddress(passwordOffset + i));
    }
    return password;
}

String readRefreshToken() {
    String token = "";
    return token;
}
