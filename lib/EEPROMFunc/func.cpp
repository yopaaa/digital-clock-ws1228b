#include "EEPROMFunc.h"
#include <EEPROM.h>

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
    Serial.println("all state is been reset...");
}

bool isEepromNotEmpty(int address)
{
    Serial.print("EEPROM number ");
    Serial.print(address);
    if (EEPROM.read(address) != 0xFF)
    {
        Serial.print(" is not empty\n");
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

    int length = value.length();

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
    Serial.println(length);
    Serial.println(address);

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

void write(int address, int value)
{
    EEPROM.put(address, value); // RED COLOR
    EEPROM.commit();
    return;
}
