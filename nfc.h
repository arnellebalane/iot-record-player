#ifndef NFC_H
#define NFC_H

// References:
// - https://kb.supremainc.com/knowledge/doku.php?id=en:1xfaq_how_to_configure_mifare_card_memory_layout
// - https://github.com/elechouse/PN532/tree/PN532_HSU

#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>

void initializeNfcReader();
String readNfcData();
uint8_t waitForNfcCard(uint8_t uid[]);
uint8_t authenticateBlock(uint8_t uid[], uint8_t blockNumber);
uint8_t readBlock(uint8_t blockNumber, uint8_t blockData[]);

#endif
