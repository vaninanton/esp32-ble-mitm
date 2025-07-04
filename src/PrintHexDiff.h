#pragma once
#include <Arduino.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
static uint8_t prevData[256];
static bool isFirst = true;

void printHex(const uint8_t* pData, size_t length) {
  for (size_t i = 0; i < length; i++) {
    Serial.printf("%02X", pData[i]);
    if (i < length - 1) {
      Serial.printf(" ");
    }
  }
}

void printHexDiff(const uint8_t* pData, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (isFirst || pData[i] == prevData[i]) {
      Serial.printf("%02X", pData[i]);
    } else {
      Serial.printf("\x1b[31m%02X\x1b[0m", pData[i]);
    }
    if (i < length - 1) {
      Serial.printf(" ");
    }
  }

  isFirst = false;
  memcpy(prevData, pData, length);
}