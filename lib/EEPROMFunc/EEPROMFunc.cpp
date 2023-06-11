#include "EEPROMFunc.h"
#include <EEPROM.h>

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

    if (isEepromNotEmpty(TIME_FORMAT_ADDRESS))
        EEPROM.read(TIME_FORMAT_ADDRESS) == 12
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

    readStaticIp();
    return;
}
