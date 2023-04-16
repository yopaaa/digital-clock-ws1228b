#include "EEPROMFunc.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "../Var.h"

void setupEEPROM()
{
    EEPROM.begin(512);
    if (EEPROM.read(1) == 0)
        defaultState();

    EEPROM.read(TIME_FORMAT_ADDRESS) == 12 ? timeFormat = 12 : timeFormat = 24;
    return;
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

void saveWifiCredentials(const char *ssid, const char *password)
{
    // Save the SSID to EEPROM
    int i = 0;
    for (; i < strlen(ssid); i++)
    {
        EEPROM.write(SSID_ADDRESS + i, ssid[i]);
    }
    for (; i < 12; i++)
    {
        EEPROM.write(SSID_ADDRESS + i, 0);
    }

    // Save the password to EEPROM
    i = 0;
    for (; i < strlen(password); i++)
    {
        EEPROM.write(PASSWORD_ADDRESS + i, password[i]);
    }
    for (; i < 32; i++)
    {
        EEPROM.write(PASSWORD_ADDRESS + i, 0);
    }

    // Commit the changes to EEPROM
    EEPROM.commit();
}

void readWifiCredentials()
{
    for (int i = SSID_ADDRESS; i < SSID_ADDRESS + 12; i++)
    {
        ssid += char(EEPROM.read(i));
    }
    for (int i = PASSWORD_ADDRESS; i < PASSWORD_ADDRESS + 32; i++)
    {
        password += char(EEPROM.read(i));
    }
}

void readColor()
{
    RED = EEPROM.read(RED_ADDRESS);
    GREEN = EEPROM.read(GREEN_ADDRESS);
    BLUE = EEPROM.read(BLUE_ADDRESS);
    return;
}


void writeIPAddressToEEPROM(int address, const IPAddress &ip)
{
    for (int i = 0; i < 4; i++)
    {
        EEPROM.write(address + i, ip[i]);
    }
}

IPAddress readIPAddressFromEEPROM(int address)
{
    byte ipAddress[4];
    for (int i = 0; i < 4; i++)
    {
        ipAddress[i] = EEPROM.read(address + i);
    }
    return IPAddress(ipAddress);
}

bool isIPAddressSet()
{
    byte ipAddress[4];
    for (int i = 0; i < 4; i++)
    {
        ipAddress[i] = EEPROM.read(IP_ADDRESS_START_ADDRESS + i);
    }
    return IPAddress(ipAddress) != IPAddress(0, 0, 0, 0);
}
