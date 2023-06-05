#include "EEPROMFunc.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "Var.h"

bool isEepromNotEmpty(int address)
{
    if (EEPROM.read(address) != 0xFF)
        return true;
        
    return false;
}

void writeString(int address, String value)
{
    EEPROM.writeString(address, value);
    EEPROM.commit();
}

void writeBool(int address, bool value)
{
    EEPROM.writeBool(address, value);
    EEPROM.commit();
}

String readString(int address)
{
    if (isEepromNotEmpty(address))
        return EEPROM.readString(address);

    return "null";
}

void defaultState()
{
    // EEPROM.put(BRIGHTNESS_ADDRESS, 100);  // SET BRIGHTNESS
    EEPROM.put(RED_ADDRESS, 255);        // RED COLOR
    EEPROM.put(GREEN_ADDRESS, 0);        // GREEN COLOR
    EEPROM.put(BLUE_ADDRESS, 0);         // BLUE COLOR
    EEPROM.put(TIME_FORMAT_ADDRESS, 24); // BLUE COLOR

    EEPROM.put(1, 1); // Setup first runing
    EEPROM.commit();
    Serial.println("all state is been reset...");
}

void saveWifiCredentials(String ssid = "max-Length-15", String password = "max-Length-32")
{
    writeString(SSID_ADDRESS, ssid);
    writeString(PASSWORD_ADDRESS, password);
}

void readWifiCredentials()
{
    ssid = readString(SSID_ADDRESS);
    password = readString(PASSWORD_ADDRESS);
}

void readColor()
{
    RED = EEPROM.readByte(RED_ADDRESS);
    GREEN = EEPROM.readByte(GREEN_ADDRESS);
    BLUE = EEPROM.readByte(BLUE_ADDRESS);
    return;
}

void writeStaticIp(const IPAddress &ip)
{
    for (int i = 0; i < 4; i++)
    {
        EEPROM.write(IP_ADDRESS_START_ADDRESS + i, ip[i]);
    }
    EEPROM.commit();
}

void readStaticIp()
{
    byte octetIp[4];

    for (int i = 0; i < 4; i++)
    {
        int address = IP_ADDRESS_START_ADDRESS + i;
        if (isEepromNotEmpty(address))
            octetIp[i] = EEPROM.read(address);
    }
    IP = IPAddress(octetIp);
    return;
}

void setupEEPROM()
{
    EEPROM.begin(512);

    if (EEPROM.read(1) == 0)
        defaultState();

    if (isEepromNotEmpty(TIME_FORMAT_ADDRESS))
        EEPROM.read(TIME_FORMAT_ADDRESS) == 12
            ? timeFormat = 12
            : timeFormat = 24;

    if (isEepromNotEmpty(IS_STATIC_IP_ADDRESS))
        isStaticIP = EEPROM.readBool(IS_STATIC_IP_ADDRESS);

    readStaticIp();
    return;
}
