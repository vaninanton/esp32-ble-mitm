#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>

extern const char* DEVICE_NAME;
extern const char* LOG_TAG;
extern const char* targetAddress;

extern NimBLEServer* appServer;
extern NimBLEClient* eucClient;
extern const NimBLEAdvertisedDevice* advDevice;

extern bool doConnect;
extern uint32_t scanTimeMs;

extern uint8_t latestPacket[512];
extern size_t latestPacketLength;