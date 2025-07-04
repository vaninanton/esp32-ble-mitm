#include "globals.h"

const char* DEVICE_NAME = "LK15332-ESP32";
const char* LOG_TAG = "ESP32-BLE-MITM";
const char* targetAddress = "88:25:83:F5:89:24";

bool doConnect = false;
uint32_t scanTimeMs = 5000;

NimBLEServer* appServer = nullptr;
NimBLEClient* eucClient = nullptr;

const NimBLEAdvertisedDevice* advDevice = nullptr;

uint8_t latestPacket[512];
size_t latestPacketLength = 0;