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

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  Wire.begin();
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DOTS_PIN, GRB>(Dots, NUM_DOTS);
  FastLED.setBrightness(BRIGHTNESS);

  if (EEPROM.read(1) == 0)
    defaultState();

  EEPROM.read(TIME_FORMAT_ADDRESS) == 12 ? timeFormat = 12 : timeFormat = 24;

  readWifiCredentials();
  pinMode(2, OUTPUT);
  pinMode(WIFI_AP_BTN, INPUT_PULLUP);
  TestStartUp();
  startWifiSta(); // STA WIFI
  httpHandler();
}

// LOOP FUNCTION
void loop()
{
  int buttonState = digitalRead(WIFI_AP_BTN);
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
