#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include "PrintHexDiff.h"
#include "globals.h"

void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic,
              uint8_t* pData,
              size_t length,
              bool isNotify) {
  if (appServer->getConnectedCount()) {
    NimBLEService* appService = appServer->getServiceByUUID("FFE0");
    if (appService) {
      NimBLECharacteristic* appChar = appService->getCharacteristic("FFE1");
      if (appChar) {
        appChar->notify(pData, length);
      }
    }
  }

  // Если пришел следующий пакет - выводим собранный до этого
  if (pData[0] == 0xDC && pData[1] == 0x5A && pData[2] == 0x5C) {
    // У нас есть пакет
    Serial.printf("[EUC] ");
    printHexDiff(latestPacket, latestPacketLength);
    Serial.printf("\n");

    latestPacketLength = 0;
  }

  if (latestPacketLength + length <= sizeof(latestPacket)) {
    // Добавляем пакет в буфер
    memcpy(&latestPacket[latestPacketLength], pData, length);
    latestPacketLength += length;
  } else {
    // Переполнение пакета, обрезаем до нуля
    latestPacketLength = 0;
    return;
  }
}