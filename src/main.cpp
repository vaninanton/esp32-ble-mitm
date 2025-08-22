#include <Arduino.h>
#include <NimBLEDevice.h>

static constexpr uint32_t scanTimeMs = 5 * 1000;
std::vector<NimBLEAdvertisedDevice> devices;

class scanCallbacks : public NimBLEScanCallbacks {
  void onDiscovered(const NimBLEAdvertisedDevice* advertisedDevice) override {
    devices.push_back(*advertisedDevice);
  }

  void onScanEnd(const NimBLEScanResults& scanResults, int reason) override {
    // Вывести список
    Serial.println("Сканирование завершено. Доступные устройства:");
    for (size_t i = 0; i < devices.size(); i++) {
      Serial.printf("[%d] Имя: '%s' MAC: %s\n", i, devices[i].getName().c_str(), devices[i].getAddress().toString().c_str());
    }

    Serial.println("Введите номер устройства для подключения:");
    while (!Serial.available()) {
      delay(100);
    }

    int index = Serial.parseInt();
    if (index >= 0 && index < (int)devices.size()) {
      Serial.printf("Введено: %d\n", index);
      Serial.printf("Подключаемся к устройству: %s\n",
                    devices[index].toString().c_str());

      NimBLEClient* pClient = NimBLEDevice::createClient();
      if (pClient->connect(&devices[index])) {
        Serial.println("Успешно подключились!");
      } else {
        Serial.println("Не удалось подключиться.");
      }
    } else {
      Serial.println("Неверный индекс.");
    }
  }
} scanCallbacks;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Инициализация BLE...");
  NimBLEDevice::init("");
  NimBLEScan* pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setScanCallbacks(&scanCallbacks, false);
  pBLEScan->setActiveScan(true);
  pBLEScan->setMaxResults(0);

  Serial.println("Сканируем устройства...");
  devices.clear();
  pBLEScan->start(scanTimeMs, false, true);
}

void loop() {
  // здесь можно добавить логику работы с подключением
}
