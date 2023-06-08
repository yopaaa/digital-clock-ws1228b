#include <Wire.h>
#include "Var.h"
#include "HttpHandler.h"
#include "Led.h"
#include "EEPROMFunc.h"
#include "WifiFunc.h"

void setup()
{
  Serial.begin(115200);
  setupEEPROM();
  Wire.begin();
  setupLED();
#if defined(ESP32)
  pinMode(WIFI_AP_BTN, INPUT_PULLDOWN);
#elif defined(ESP8266)
  pinMode(WIFI_AP_BTN, INPUT_PULLDOWN_16);
#endif
  TestStartUp();
  readWifiCredentials();
  digitalRead(WIFI_AP_BTN) == HIGH ? startWifiAp() : startWifiSta();
  httpHandler();
}

void loop()
{
  if (ColorMode == "random")
  {
    RED = random(1, 255);
    GREEN = random(1, 255);
    BLUE = random(1, 255);
  }

  mainLoop();
}
