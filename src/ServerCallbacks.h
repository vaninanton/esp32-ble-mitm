#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include "globals.h"

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    Serial.printf("[APP] Client connected! %s (%d/%d/%d/%d)\n",
                  connInfo.getAddress().toString().c_str(),
                  connInfo.getConnInterval(), connInfo.getConnTimeout(),
                  connInfo.getConnLatency(), connInfo.getMTU());
    // pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
  }

  void onDisconnect(NimBLEServer* pServer,
                    NimBLEConnInfo& connInfo,
                    int reason) override {
    Serial.printf("[APP] Client disconnected - start advertising\n");
    NimBLEDevice::startAdvertising();
  }

  void onConnParamsUpdate(NimBLEConnInfo& connInfo) {
    Serial.printf("[APP] Connection params updated! %s (%d/%d/%d/%d)\n",
                  connInfo.getAddress().toString().c_str(),
                  connInfo.getConnInterval(), connInfo.getConnTimeout(),
                  connInfo.getConnLatency(), connInfo.getMTU());
  }

  void onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) override {
    Serial.printf("[APP] Server MTU changed to %u\n", MTU);
    NimBLEDevice::setMTU(MTU);
    eucClient->exchangeMTU();
  }
} serverCallbacks;