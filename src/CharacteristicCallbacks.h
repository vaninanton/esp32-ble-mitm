#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include "PrintHexDiff.h"
#include "globals.h"

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onRead(NimBLECharacteristic* pCharacteristic,
              NimBLEConnInfo& connInfo) override {
    Serial.printf("[APP] Read from %s: \"%s\"\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
  }

  void onWrite(NimBLECharacteristic* pCharacteristic,
               NimBLEConnInfo& connInfo) override {
    NimBLEUUID serviceUUID = pCharacteristic->getService()->getUUID();
    NimBLEUUID charUUID = pCharacteristic->getUUID();
    const uint8_t* pRaw = pCharacteristic->getValue().data();
    const size_t pSize = pCharacteristic->getLength();

    Serial.printf("[APP] ");
    printHex(pRaw, pSize);
    Serial.printf("\n");

    if (eucClient->isConnected() == false) {
      Serial.printf("[APP] EUC is not connected\n");
      return;
    }

    NimBLERemoteService* pSvc = eucClient->getService(serviceUUID);
    if (pSvc == nullptr) {
      Serial.printf("[APP] EUC service not found\n");
      return;
    }

    NimBLERemoteCharacteristic* pChr = pSvc->getCharacteristic(charUUID);
    if (pChr == nullptr) {
      Serial.printf("[APP] EUC characteristic not found\n");
      return;
    }

    if (pChr->canWrite() == false) {
      Serial.printf("[APP] EUC characteristic cannot be written\n");
      return;
    }

    bool wrote = pChr->writeValue(pRaw, pSize, false);
    if (!wrote) {
      Serial.printf("[APP] Error while wrote new value to client\n");
    }
  }

  //   /**
  //    *  The value returned in code is the NimBLE host return code.
  //    */
  //   void onStatus(NimBLECharacteristic* pCharacteristic, int code) override {
  //     Serial.printf("[APP] OnStatus return code: %d, %s\n", code,
  //                   NimBLEUtils::returnCodeToString(code));
  //   }

  /** Peer subscribed to notifications/indications */
  void onSubscribe(NimBLECharacteristic* pCharacteristic,
                   NimBLEConnInfo& connInfo,
                   uint16_t subValue) override {
    std::string str = "[APP] ";
    str += "Client ";
    str += connInfo.getAddress().toString();
    if (subValue == 0) {
      str += " unsubscribed to ";
    } else if (subValue == 1) {
      str += " subscribed to notifications for ";
    } else if (subValue == 2) {
      str += " subscribed to indications for ";
    } else if (subValue == 3) {
      str += " subscribed to notifications and indications for ";
    }
    str += std::string(pCharacteristic->getUUID());

    Serial.printf("%s\n", str.c_str());
  }
} chrCallbacks;
