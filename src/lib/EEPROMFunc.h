#ifndef EEPROM_FUNC
#define EEPROM_FUNC

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void setupEEPROM();

void saveWifiCredentials(const char *ssid, const char *password);
void defaultState();
void readWifiCredentials();

void readColor();
void writeIPAddressToEEPROM(int address, const IPAddress &ip);
IPAddress readIPAddressFromEEPROM(int address);
bool isIPAddressSet();

#endif