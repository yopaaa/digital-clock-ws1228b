#ifndef EEPROM_FUNC
#define EEPROM_FUNC

#include <Arduino.h>

void setupEEPROM();

void saveWifiCredentials(String ssid, String password);
void defaultState();
void readWifiCredentials();

void readColor();
void writeStaticIp(const IPAddress &ip);
void writeString(int address, String value);
void writeBool(int address, bool value);
void writeLong(int address, long value);
String readString(int address);

#endif