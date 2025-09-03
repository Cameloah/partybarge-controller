#include <Arduino.h>
#include "main.h"
#include "main_project_utils.h"
#include <daly-bms-uart.h>

#define BMS_SERIAL Serial2
Daly_BMS_UART bms(BMS_SERIAL);

unsigned long lastUpdate = 0;

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", String(bms.get.packSOC));
}

void setup() {
  DualSerial.begin(115200);
  DualSerial.println("Starting Wifi...");
  project_utils_init("Partybarge");
  server.on("/", HTTP_GET, handleRoot);

  bms.Init();
}

void loop() {
  project_utils_update();

  if (millis() - lastUpdate >= 1000) {
      bms.update();
      DualSerial.println("Basic BMS Data:              " + (String)bms.get.packVoltage + "V " + (String)bms.get.packCurrent + "I " + (String)bms.get.packSOC + "\% ");
      DualSerial.println("Min/Max Cell Voltage:       " + (String)bms.get.minCellmV + "mV " + (String)bms.get.minCellVNum + " " + (String)bms.get.maxCellmV + "mV " + (String)bms.get.maxCellVNum);
    lastUpdate = millis();
  }  
}