#include <EEPROM.h>
#include "Storage.h"
#include "Mode.h"
#include "WifiFunc.h"
#include "Led.h"
#include "ADDRESS.h"

void beginEEPROM(int size)
{
    EEPROM.begin(size);
    return;
}

bool isEepromNotEmpty(int address)
{
    if (EEPROM.read(address) != 0xFF)
    {
        return true;
    }
    Serial.print("EEPROM number ");
    Serial.print(address);
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

    int length = value.length();
    if (length > 30)
        return;

    // Write length of string
    EEPROM.write(address, length);
    address++;

    // Write string characters
    for (int i = 0; i < length; i++)
    {
        EEPROM.write(address + i, value[i]);
    }

    // Commit changes to EEPROM
    EEPROM.commit();
}
String readString(int address)
{
    Serial.print("Reading Strings on address ");
    Serial.print(address);
    Serial.print("\n");

    int length = EEPROM.read(address);

    address++;

    // Read string characters
    char charData[length + 1];
    for (int i = 0; i < length; i++)
    {
        charData[i] = EEPROM.read(address + i);
    }
    charData[length] = '\0';

    String data = String(charData);

    return data;
}

void writeBool(int address, bool value)
{
    byte boolValue = value ? 1 : 0;

    EEPROM.write(address, boolValue);
    EEPROM.commit();

    Serial.print("successful writing ");
    Serial.print(value);
    Serial.print(" to address ");
    Serial.print(address);
    Serial.println();
}
bool readBool(int address)
{
    byte boolValue = EEPROM.read(address);
    return boolValue != 0;
}

void writeLong(int address, long value)
{
    byte *valuePtr = (byte *)&value;
    for (size_t i = 0; i < sizeof(long); i++)
    {
        EEPROM.write(address + i, valuePtr[i]);
    }
    EEPROM.commit();

    Serial.print("successful writing ");
    Serial.print(value);
    Serial.print(" to address ");
    Serial.print(address);
    Serial.println();
}
long readLong(int address)
{
    long value = 0;
    byte *valuePtr = (byte *)&value;
    for (size_t i = 0; i < sizeof(long); i++)
    {
        valuePtr[i] = EEPROM.read(address + i);
    }
    return value;
}

void writeByte(int address, byte value)
{
    EEPROM.write(address, value);
    EEPROM.commit();
}
byte readByte(int address)
{
    return EEPROM.read(address);
}

int read(int address)
{
    return EEPROM.read(address);
}
void write(int address, int value)
{
    EEPROM.put(address, value);
    EEPROM.commit();
    return;
}

void resetEEPROM()
{
    int size = EEPROM.length();
    for (int i = 0; i < size; i++)
    {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.put(RED_ADDRESS, 255);
    EEPROM.put(GREEN_ADDRESS, 0);
    EEPROM.put(BLUE_ADDRESS, 0);
    EEPROM.put(TIME_FORMAT_ADDRESS, 24);
    EEPROM.commit();

    writeBool(511, true);
    writeLong(GMT_OFFSET_ADDRESS, 7 * 3600);
    writeString(AP_SSID_ADDRESS, "digital_clock");
    writeString(AP_PASSWORD_ADDRESS, "12345678zxcvbnm");
    Serial.println("all state is been reset...");
}

void readWifiCredentials()
{
    if (isEepromNotEmpty(SSID_ADDRESS))
        ssid = readString(SSID_ADDRESS);

    if (isEepromNotEmpty(PASSWORD_ADDRESS))
        password = readString(PASSWORD_ADDRESS);

    if (isEepromNotEmpty(AP_SSID_ADDRESS))
        APssid = readString(AP_SSID_ADDRESS);

    if (isEepromNotEmpty(AP_PASSWORD_ADDRESS))
        APpassword = readString(AP_PASSWORD_ADDRESS);
}

void readColor()
{
    RED = readByte(RED_ADDRESS);
    GREEN = readByte(GREEN_ADDRESS);
    BLUE = readByte(BLUE_ADDRESS);
    return;
}

void writeStaticIp(const IPAddress &ip)
{
    for (int i = 0; i < 4; i++)
    {
        write(IP_ADDRESS_START_ADDRESS + i, ip[i]);
    }
}

void readStaticIp()
{
    byte octetIp[4];

    for (int i = 0; i < 4; i++)
    {
        int address = IP_ADDRESS_START_ADDRESS + i;
        if (isEepromNotEmpty(address))
            octetIp[i] = read(address);
    }
    IP = IPAddress(octetIp);
    return;
}

void setupEEPROM()
{
    beginEEPROM(512);

    if (!isEepromNotEmpty(511))
        resetEEPROM();

    if (isEepromNotEmpty(TIME_FORMAT_ADDRESS))
        read(TIME_FORMAT_ADDRESS) == 12
            ? timeFormat = 12
            : timeFormat = 24;

    if (isEepromNotEmpty(IS_STATIC_IP_ADDRESS))
        isStaticIP = readBool(IS_STATIC_IP_ADDRESS);

    if (isEepromNotEmpty(GMT_OFFSET_ADDRESS))
        gmtOffset_sec = readLong(GMT_OFFSET_ADDRESS);

    if (isEepromNotEmpty(SEGMENT_1_MODE_ADDRESS))
        segment1mode = readString(SEGMENT_1_MODE_ADDRESS);
    if (isEepromNotEmpty(SEGMENT_2_MODE_ADDRESS))
        segment2mode = readString(SEGMENT_2_MODE_ADDRESS);

    if (isStaticIP)
        readStaticIp();
    
    return;
}
