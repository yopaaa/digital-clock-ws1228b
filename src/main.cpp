#include "Arduino.h"
#include <EEPROM.h>
#include <time.h>
#include <FastLED.h>
#include <Wire.h>
#include <FS.h>
#include "Var.h"
#include "./lib/HttpHandler.h"
#include "./lib/Led.h"
#include "./lib/EEPROMFunc.h"
#include "./lib/WifiFunc.h"

// SETUP FUNCTION
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  Wire.begin();
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DOTS_PIN, GRB>(Dots, NUM_DOTS);
  FastLED.setBrightness(BRIGHTNESS); // SET BRIGHTNESS FROM EEPROM VALUE

  if (EEPROM.read(1) == 0)
    defaultState(); // TO SETUP FIRST RUNING EEPROM
  if (EEPROM.read(TIME_FORMAT_ADDRESS) == 0)
  {
    timeFormat = 24;
  }
  else
  {
    timeFormat = EEPROM.read(TIME_FORMAT_ADDRESS);
  }

  // Read the saved SSID and password from EEPROM
  readWifiCredentials();
  

  pinMode(2, OUTPUT);
  pinMode(startWifiApBtn, INPUT_PULLUP);

  TestStartUp();
  startWifiSta(); // STA WIFI
  httpHandler();
}

// LOOP FUNCTION
void loop()
{
  int buttonState = digitalRead(startWifiApBtn);
  if (buttonState == LOW)
  {
    startWifiAp();
  }

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
