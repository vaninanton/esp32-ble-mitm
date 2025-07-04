#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include "ClientCallbacks.h"
#include "NotifyCB.h"
#include "globals.h"

bool connectToServer() {
  eucClient = nullptr;

  if (NimBLEDevice::getCreatedClientCount()) {
    eucClient = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());
    if (eucClient) {
      if (!eucClient->connect(advDevice, false)) {
        Serial.printf("[EUC] Reconnect failed\n");
        return false;
      }
      Serial.printf("[EUC] Reconnected client\n");
    } else {
      /**
       * We don't already have a client that knows this device,
       * check for a client that is disconnected that we can use.
       */
      eucClient = NimBLEDevice::getDisconnectedClient();
    }
  }

  /** No client to reuse? Create a new one. */
  if (!eucClient) {
    if (NimBLEDevice::getCreatedClientCount() >= NIMBLE_MAX_CONNECTIONS) {
      Serial.printf(
          "[EUC] Max clients reached - no more connections available\n");
      return false;
    }

    eucClient = NimBLEDevice::createClient();
    Serial.printf("[EUC] New client created\n");

    eucClient->setClientCallbacks(&clientCallbacks, false);

    /**
     *  Set initial connection parameters:
     *  These settings are safe for 3 clients to connect reliably, can go faster
     * if you have less connections. Timeout should be a multiple of the
     * interval, minimum is 100ms. Min interval: 12 * 1.25ms = 15, Max interval:
     * 12 * 1.25ms = 15, 0 latency, 150 * 10ms = 1500ms timeout
     */
    // eucClient->setConnectionParams(12, 12, 0, 150);

    /** Set how long we are willing to wait for the connection to complete
     * (milliseconds), default is 30000. */
    // eucClient->setConnectTimeout(5 * 1000);

    if (!eucClient->connect(advDevice)) {
      /** Created a client but failed to connect, don't need to keep it as it
       * has no data */
      NimBLEDevice::deleteClient(eucClient);
      Serial.printf("[EUC] Failed to connect, deleted client\n");
      return false;
    }
  }

  if (!eucClient->isConnected()) {
    if (!eucClient->connect(advDevice)) {
      Serial.printf("[EUC] Failed to connect\n");
      return false;
    }
  }

  Serial.printf("[EUC] Connected to: %s RSSI: %d\n",
                eucClient->getPeerAddress().toString().c_str(),
                eucClient->getRssi());

  eucClient->exchangeMTU();

  /** Now we can read/write/subscribe the characteristics of the services we are
   * interested in */
  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;

  pSvc = eucClient->getService("FFE0");
  if (pSvc) {
    pChr = pSvc->getCharacteristic("FFE1");
    if (pChr && pChr->canNotify()) {
      if (!pChr->subscribe(true, notifyCB)) {
        eucClient->disconnect();
        return false;
      }
    } else {
      Serial.printf("[EUC] FFE1 characteristic not found.\n");
    }
  }

  Serial.printf("[EUC] Done with this device!\n");
  return true;
}