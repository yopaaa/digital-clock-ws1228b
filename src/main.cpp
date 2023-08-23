#include <Wire.h>
#include "Led.h"
#include "Storage.h"
#include "WifiFunc.h"
#include "Alarm.h"
#include "Mode.h"
#include "mqtt.h"

void setup()
{
  Serial.begin(115200);
  setupEEPROM();
  Wire.begin();
  setupLED();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

#if defined(ESP32)
  pinMode(WIFI_AP_BTN, INPUT_PULLDOWN);
#elif defined(ESP8266)
  pinMode(WIFI_AP_BTN, INPUT);
#endif
  BlankDots();
  TestStartUp();
  readWifiCredentials();
  digitalRead(WIFI_AP_BTN) == HIGH ? startWifiAp() : startWifiSta();

  // setAlarm
  if (isEepromNotEmpty(ALARM_LIST_ADDRESS))
    readAlarmsFromEEPROM();
}

void isRandomColorMode()
{
  if (colorMode == "random")
  {
    RED = random(1, 255);
    GREEN = random(1, 255);
    BLUE = random(1, 255);
  }
}

void loop()
{
  mqttLoop();
  isRandomColorMode();
  mainLoop();
}
