#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include "PrintHexDiff.h"
#include "globals.h"

class ScanCallbacks : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
    Serial.printf("[EUC] Device found: \"%s\" (%s)\n",
                  advertisedDevice->getName().c_str(),
                  advertisedDevice->getAddress().toString().c_str());
    if (advertisedDevice->haveName() &&
        advertisedDevice->getName() == "LK15332") {
      /** stop scan before connecting */
      NimBLEDevice::getScan()->stop();

      /** Save the device reference in a global for the client to use*/
      advDevice = advertisedDevice;

      /** Ready to connect now */
      doConnect = true;
    }
  }

  /** Callback to process the results of the completed scan or restart it */
  void onScanEnd(const NimBLEScanResults& results, int reason) override {
    // Serial.printf("[EUC] Scan Ended, reason: %d, device count: %d; Restarting
    // scan\n", reason, results.getCount());
    NimBLEDevice::getScan()->start(scanTimeMs, false, true);
  }
} scanCallbacks;