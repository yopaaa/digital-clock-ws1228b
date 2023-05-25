#ifndef WIFI_FUNC
#define WIFI_FUNC

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

void startWifiAp();
void startWifiSta();
bool isInternetConnection();
void checkReConnectWifiSta();

#endif