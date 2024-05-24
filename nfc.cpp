#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include "nfc.h"
#include "secrets.h"

PN532_SPI spi(SPI, 2);
PN532 nfc(spi);

uint8_t supportedUidLength = 4;
uint8_t blockLimit = 16 * 4;
uint8_t blockSize = 16;

void initializeNfcReader() {
    nfc.begin();

    uint32_t firmwareVersion = nfc.getFirmwareVersion();
    if (firmwareVersion) {
        Serial.print("NFC reader found: PN5");
        Serial.println((firmwareVersion >> 24) & 0xff, HEX);
        Serial.print("Firmware version: ");
        Serial.print((firmwareVersion >> 16) & 0xff, DEC);
        Serial.print(".");
        Serial.println((firmwareVersion >> 8) & 0xff, DEC);
    } else {
        Serial.println("NFC reader not found");
        while (1);
    }

    nfc.SAMConfig();
    Serial.println("NFC reader is now ready");
}

String readNfcData() {
    String data = "";
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
    bool complete = false;

    if (waitForNfcCard(uid)) {
        // Mifare 1K cards have 16 sectors, 4 blocks per sector, and 16 bytes per block.
        // Keep reading each byte until we reach the end or until no data is found.
        // The first sector usually stores the CIS blocks which contains information about the card, access group, etc.
        // so we skip the first sector and start reading from block number 4
        uint8_t blockNumber = 4;
        uint8_t blockData[blockSize];

        while (blockNumber < blockLimit && !complete) {
            if (!authenticateBlock(uid, blockNumber)) {
                Serial.print("Failed to authenticate block number: ");
                Serial.println(blockNumber);
                return "";
            }

            readBlock(blockNumber, blockData);
            nfc.PrintHexChar(blockData, blockSize);

            for (uint8_t i = 0; i < blockSize; i++) {
                if (blockData[i] == 0) {
                    complete = true;
                    break;
                }
                data += (char) blockData[i];
            }
            blockNumber++;
        }
    }

    return data;
}

uint8_t waitForNfcCard(uint8_t uid[]) {
    uint8_t uidLength;

    // Wait for an ISO14443A type card (Mifare, etc.) and store its UID data in `uid` and `uidLength` variables.
    // UID length == 4 -> Mifare Classic (1024 bytes memory)
    // UID length == 7 -> Mifare Ultralight (we will not support this)
    uint8_t success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (success) {
        Serial.print("Card found with UID: ");
        nfc.PrintHex(uid, uidLength);

        if (uidLength != supportedUidLength) {
            Serial.println("Card is not supported");
            return 0;
        }
        Serial.println("Card is supported");
    }
    return success;
}

uint8_t authenticateBlock(uint8_t uid[], uint8_t blockNumber) {
    Serial.print("Authenticating block number: ");
    Serial.println(blockNumber);

    return nfc.mifareclassic_AuthenticateBlock(uid, supportedUidLength, blockNumber, 0, NFC_AUTHENTICATION_KEY);
}

uint8_t readBlock(uint8_t blockNumber, uint8_t blockData[]) {
    Serial.print("Reading block number: ");
    Serial.println(blockNumber);

    return nfc.mifareclassic_ReadDataBlock(blockNumber, blockData);
}
