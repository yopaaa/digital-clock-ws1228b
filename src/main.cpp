#include "Arduino.h"
#include <Wire.h>
#include "Var.h"
#include "./lib/HttpHandler.h"
#include "./lib/Led.h"
#include "./lib/EEPROMFunc.h"
#include "./lib/WifiFunc.h"

void setup()
{
  Serial.begin(9600);
  setupEEPROM();
  Wire.begin();
  setupLED();
  pinMode(WIFI_AP_BTN, INPUT_PULLUP);
  TestStartUp();
  readWifiCredentials();

  startWifiSta(); // STA WIFI
  httpHandler();
}

// LOOP FUNCTION
void loop()
{
  if (displayMode == "clock")
  {
    printLocalTime();
    delay(RefreshDelay);
  }
  else if (displayMode == "counter")
  {
    printCounter();
    delay(1000);
  }
  else if (displayMode == "countdown")
  {
    printCountDown();
    delay(1000);
  }
}
