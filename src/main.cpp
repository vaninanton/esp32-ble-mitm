#include <Arduino.h>
#include <NimBLEDevice.h>
#include <globals.h>
#include "CharacteristicCallbacks.h"
#include "ClientCallbacks.h"
#include "ConnectToServer.h"
#include "NotifyCB.h"
#include "ScanCallbacks.h"
#include "ServerCallbacks.h"

void setup() {
  Serial.begin(115200);
  Serial.printf("[ESP] Starting application\n");

  /** Initialize NimBLE and set the device name */
  NimBLEDevice::init(DEVICE_NAME);
  NimBLEDevice::setPower(3);
  NimBLEDevice::setMTU(BLE_ATT_MTU_MAX);

  appServer = NimBLEDevice::createServer();
  appServer->setCallbacks(&serverCallbacks);

  NimBLEService* appService = appServer->createService("FFE0");
  NimBLECharacteristic* appChar = appService->createCharacteristic(
      "FFE1", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                  NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY);
  appChar->setCallbacks(&chrCallbacks);

  /** Start the services when finished creating all Characteristics and
   * Descriptors */
  appService->start();

  /** Create an advertising instance and add the services to the advertised data
   */
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setName(DEVICE_NAME);
  pAdvertising->addServiceUUID(appService->getUUID());
  // pAdvertising->setManufacturerData("484d882583f58924");

  /**
   *  If your device is battery powered you may consider setting scan response
   *  to false as it will extend battery life at the expense of less data sent.
   */
  pAdvertising->enableScanResponse(true);
  pAdvertising->start();

  Serial.printf("[ESP] Advertising Started\n");

  /** Optional: set the transmit power */
  NimBLEDevice::setPower(3); /** 3dbm */
  NimBLEScan* pScan = NimBLEDevice::getScan();

  /** Set the callbacks to call when scan events occur, no duplicates */
  pScan->setScanCallbacks(&scanCallbacks, false);

  // /** Set scan interval (how often) and window (how long) in milliseconds */
  pScan->setInterval(100);
  pScan->setWindow(100);

  /**
   * Active scan will gather scan response data from advertisers
   *  but will use more energy from both devices
   */
  pScan->setActiveScan(true);

  Serial.printf("[ESP] Application started\n");

  /** Start scanning for advertisers */
  pScan->start(scanTimeMs);

  Serial.printf("[ESP] Scanning...\n");
}

void loop() {
  /** Loop here until we find a device we want to connect to */
  delay(10);

  if (doConnect) {
    doConnect = false;
    /** Found a device we want to connect to, do it now */
    if (connectToServer()) {
      // Serial.printf("Success! Scanning for more!\n");
    } else {
      Serial.printf("Failed to connect, starting scan\n");
    }

    NimBLEDevice::getScan()->start(scanTimeMs, false, true);
  }
}
