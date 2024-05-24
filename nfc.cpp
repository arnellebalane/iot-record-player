#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include "nfc.h"
#include "secrets.h"

PN532_SPI spi(SPI, 2);
PN532 nfc(spi);

uint8_t supportedUidLength = 4;
uint8_t blockSize = 16;

// Mifare 1K cards have 16 sectors, 4 blocks per sector, and 16 bytes per block.
uint8_t blockNumberLimit = 16 * 4;

// The first sector usually stores the CIS blocks which contains information about the card, access group, etc.
// so we skip the first sector and start reading from block number 4
uint8_t blockNumberInitial = 4;

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

    if (waitForNfcCard(uid)) {
        uint8_t blockNumber = blockNumberInitial;
        uint8_t blockData[blockSize];
        bool complete = false;

        // Keep reading each byte until we reach the end or until no data is found.
        while (blockNumber < blockNumberLimit && !complete) {
            if (!authenticateBlock(uid, blockNumber)) {
                return "";
            }

            readBlock(blockNumber, blockData);

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

uint8_t writeNfcData(String data) {
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };

    if (waitForNfcCard(uid)) {
        uint16_t dataLength = data.length();
        uint8_t blockNumber = blockNumberInitial;
        uint8_t blockData[blockSize];
        bool complete = false;

        while (blockNumber < blockNumberLimit && !complete) {
            if (!authenticateBlock(uid, blockNumber)) {
                return 0;
            }

            for (uint8_t i = 0; i < blockSize; i++) {
                uint16_t dataIndex = (blockNumber - blockNumberInitial) * blockSize + i;
                if (dataIndex < dataLength) {
                    blockData[i] = (uint8_t) data.charAt(dataIndex);
                } else {
                    complete = true;
                    blockData[i] = 0;
                }

            }

            writeBlock(blockNumber, blockData);

            blockNumber++;
        }
        return 1;
    }
    return 0;
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

    uint8_t success = nfc.mifareclassic_AuthenticateBlock(uid, supportedUidLength, blockNumber, 0, NFC_AUTHENTICATION_KEY);
    if (!success) {
        Serial.print("Failed to authenticate block number: ");
        Serial.println(blockNumber);
    }
    return success;
}

uint8_t readBlock(uint8_t blockNumber, uint8_t blockData[]) {
    Serial.print("Reading from block number: ");
    Serial.println(blockNumber);

    uint8_t success = nfc.mifareclassic_ReadDataBlock(blockNumber, blockData);
    if (success) {
        nfc.PrintHexChar(blockData, blockSize);
    }
    return success;
}

uint8_t writeBlock(uint8_t blockNumber, uint8_t blockData[]) {
    Serial.print("Writing to block number: ");
    Serial.println(blockNumber);
    nfc.PrintHexChar(blockData, blockSize);

    return nfc.mifareclassic_WriteDataBlock(blockNumber, blockData);
}
