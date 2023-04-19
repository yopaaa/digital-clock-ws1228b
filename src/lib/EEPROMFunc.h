#ifndef EEPROM_FUNC
#define EEPROM_FUNC

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void setupEEPROM();

void saveWifiCredentials(String ssid, String password);
void defaultState();
void readWifiCredentials();

void readColor();
void writeStaticIp(const IPAddress &ip, const IPAddress &gateway, const IPAddress &subnet);
void writeString(int address, String value);
void writeBool(int address, bool value);
String readString(int address);

#endif