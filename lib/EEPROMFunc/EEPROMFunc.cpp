#include "EEPROMFunc.h"
#include <EEPROM.h>
#include "Var.h"

bool isEepromNotEmpty(int address)
{
    Serial.print("EEPROM number ");
    Serial.print(address);
    if (EEPROM.read(address) != 0xFF)
    {
        Serial.print("is not empty\n");
        return true;
    }

    Serial.print(" is empty\n");
    return false;
}

void writeString(int address, String value)
{
    Serial.print("successful writing ");
    Serial.print(value);
    Serial.print(" to address ");
    Serial.print(address);
    Serial.println();

    EEPROM.writeString(address, value);
    EEPROM.commit();
}

void writeBool(int address, bool value)
{
    Serial.print("successful writing ");
    Serial.print(value);
    Serial.print(" to address ");
    Serial.print(address);
    Serial.println();

    EEPROM.writeBool(address, value);
    EEPROM.commit();
}

void writeLong(int address, long value)
{
    Serial.print("successful writing ");
    Serial.print(value);
    Serial.print(" to address ");
    Serial.print(address);
    Serial.println();

    EEPROM.writeLong(address, value);
    EEPROM.commit();
}

String readString(int address)
{
    Serial.print("Reading ");
    Serial.print(address);
    Serial.print("\n");
    if (isEepromNotEmpty(address))
    {
        String x = EEPROM.readString(address);
        return x;
    }
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

    if (isEepromNotEmpty(GMT_OFFSET_ADDRESS))
        gmtOffset_sec = EEPROM.readLong(GMT_OFFSET_ADDRESS);

    if (isEepromNotEmpty(SEGMENT_1_MODE_ADDRESS))
        segment1mode = EEPROM.readString(SEGMENT_1_MODE_ADDRESS);
    if (isEepromNotEmpty(SEGMENT_2_MODE_ADDRESS))
        segment2mode = EEPROM.readString(SEGMENT_2_MODE_ADDRESS);

    readStaticIp();
    return;
}
