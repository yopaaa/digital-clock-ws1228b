#ifndef EEPROM_FUNC
#define EEPROM_FUNC

#include <Arduino.h>
#include <IPAddress.h>
#define PRINT_VARIABLE_NAME(var) Serial.print(#var)

void setupEEPROM();
void readWifiCredentials();
void readColor();
void writeStaticIp(const IPAddress &ip);

void beginEEPROM(int size);
void resetEEPROM();

bool isEepromNotEmpty(int address);

void write(int address, int value);
int read(int address);

void writeString(int address, String value);
String readString(int address);

void writeBool(int address, bool value);
bool readBool(int address);

void writeLong(int address, long value);
long readLong(int address);

void writeByte(int address, byte value);
byte readByte(int address);

#endif