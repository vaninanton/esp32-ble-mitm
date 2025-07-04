#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include "globals.h"

class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) override {
    Serial.printf("[EUC] Connected\n");
  }

  void onDisconnect(NimBLEClient* pClient, int reason) override {
    Serial.printf("[EUC] %s Disconnected, reason = %d - Starting scan\n",
                  pClient->getPeerAddress().toString().c_str(), reason);
    NimBLEDevice::getScan()->start(scanTimeMs, false, true);
  }

  bool onConnParamsUpdateRequest(NimBLEClient* pClient,
                                 const ble_gap_upd_params* params) override {
    Serial.printf("onConnParamsUpdateRequest: %d/%d/%d/%d/%d/%d\n",
                  params->itvl_min, params->itvl_max, params->latency,
                  params->supervision_timeout, params->min_ce_len,
                  params->max_ce_len);
    return true;
  }

  void onMTUChange(NimBLEClient* pClient, uint16_t MTU) override {
    Serial.printf("[EUC] Client MTU changed to %u\n", MTU);
  }

  void onPhyUpdate(NimBLEClient* pClient,
                   uint8_t txPhy,
                   uint8_t rxPhy) override {
    Serial.printf("[EUC] Client PHY changed to %u, %u\n", txPhy, rxPhy);
  }
} clientCallbacks;