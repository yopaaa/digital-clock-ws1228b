#include <EEPROM.h>
#include "Alarm.h"
#include "Buzzer.h"
#include "ADDRESS.h"

Alarm alarms[MAX_ALARMS] = {};

// function to convert binary array to hexadecimal like {0,1,1,1,1,1,0} > 62
void hexToBinaryArray(unsigned int hexValue, int *binaryArray)
{
    for (int i = 7 - 1; i >= 0; i--)
    {
        binaryArray[i] = hexValue & 0x01;
        hexValue >>= 1;
    }
}

// function to convert hexadecimal to binary array like 62 > {0,1,1,1,1,1,0}
unsigned int binaryArrayToHex(const int *binaryArray)
{
    unsigned int hexValue = 0;

    for (int i = 0; i < 7; i++)
    {
        hexValue |= binaryArray[i] << (7 - i - 1);
    }

    return hexValue;
}

// function to write/saved alarm to eeprom.
void writeAlarmsToEEPROM()
{
    int address = ALARM_LIST_ADDRESS;

    for (int i = 0; i < MAX_ALARMS; i++)
    {
        Serial.print("\nwriting alarms ");
        Serial.print(i);
        Serial.print(" to ");
        Serial.print(address);

        EEPROM.write(address, alarms[i].hour);
        EEPROM.write(address + 1, alarms[i].min);
        EEPROM.write(address + 2, alarms[i].alertIndex);
        EEPROM.write(address + 3, binaryArrayToHex(alarms[i].days));
        address += 4;
    }
    EEPROM.commit();
}

// function to read saved alarm from eeprom.
void readAlarmsFromEEPROM()
{
    int address = ALARM_LIST_ADDRESS;

    for (int i = 0; i < MAX_ALARMS; i++)
    {
        Serial.print("\nreading alarms ");
        Serial.print(i);
        Serial.print(" to ");
        Serial.print(address);

        alarms[i].hour = EEPROM.read(address);
        alarms[i].min = EEPROM.read(address + 1);
        alarms[i].alertIndex = EEPROM.read(address + 2);

        int binaryArray[7] = {0};
        hexToBinaryArray(EEPROM.read(address + 3), binaryArray);
        alarms[i].days[0] = binaryArray[0];
        alarms[i].days[1] = binaryArray[1];
        alarms[i].days[2] = binaryArray[2];
        alarms[i].days[3] = binaryArray[3];
        alarms[i].days[4] = binaryArray[4];
        alarms[i].days[5] = binaryArray[5];
        alarms[i].days[6] = binaryArray[6];

        address += 4;
    }
}

// function to update or create a new alarm.
void setAlarm(int index, int days[7], int hour, int min, int alertIndex)
{
    Serial.print("\nset alarms ");
    Serial.print(index);
    Serial.print(" to hour");
    Serial.print(hour);

    alarms[index].hour = hour;
    alarms[index].min = min;
    alarms[index].alertIndex = alertIndex;

    alarms[index].days[0] = days[0];
    alarms[index].days[1] = days[1];
    alarms[index].days[2] = days[2];
    alarms[index].days[3] = days[3];
    alarms[index].days[4] = days[4];
    alarms[index].days[5] = days[5];
    alarms[index].days[6] = days[6];
}

bool buzzerState = true;
void playAlarm(int gap) // function to play alarm
{
    if (buzzerState)
    {
        myBuzzer.on();
    }
    else
    {
        myBuzzer.off();
    }

    buzzerState = !buzzerState;
    delay(gap);
}

// function to check if there is an alarm at the current time.
void checkAlarm(int day, int hour, int minute)
{
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        if ((alarms[i].hour == hour) && (alarms[i].min == minute) && (alarms[i].days[6 - day] == 1))
        {
            if (alarms[i].alertIndex == 1)
            {
                Serial.print("\nAlert alarm ");
                Serial.print(i);
                playAlarm(0);
            }

            return;
        }
    }
    myBuzzer.off();
}
