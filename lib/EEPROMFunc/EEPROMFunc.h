#ifndef EEPROM_FUNC
#define EEPROM_FUNC

#include <Arduino.h>
#include "Var.h"

void setupEEPROM();

void readWifiCredentials();

void readColor();
void resetEEPROM();
bool isEepromNotEmpty(int address);
void write(int address, int value);
void writeStaticIp(const IPAddress &ip);
void writeString(int address, String value);
void writeBool(int address, bool value);
void writeLong(int address, long value);
String readString(int address);
bool readBool(int address);
long readLong(int address);
byte readByte(int address);

#endif