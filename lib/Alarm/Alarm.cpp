#include <EEPROM.h>
#include <Var.h>
#include "Alarm.h"
#include "Buzzer.h"

Alarm alarms[MAX_ALARMS] = {};

// Fungsi untuk menulis data alarm ke EEPROM
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
        address += 2;
    }
    EEPROM.commit();
}

// Fungsi untuk membaca data alarm dari EEPROM
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
        address += 2;
    }
}

// Fungsi untuk mengatur alarm pada indeks tertentu
void setAlarm(int index, int hour, int min)
{
    Serial.print("\nset alarms ");
    Serial.print(index);
    Serial.print(" to hour");
    Serial.print(hour);

    alarms[index].hour = hour;
    alarms[index].min = min;
}

int indexOfHour(int hour)
{
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        if (alarms[i].hour == hour)
        {
            return i; // Mengembalikan indeks alarm yang sesuai
        }
    }
    return -1; // Mengembalikan -1 jika tidak ditemukan
}

bool buzzerState = true;
void playAlarm(int gap)
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

void checkAlarm(int hour, int minute)
{
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        if ((alarms[i].hour == hour) && (alarms[i].min == minute))
        {
            Serial.print("\nAlert alarm ");
            Serial.print(i);
            playAlarm(0);
            return;
        }
    }
    myBuzzer.off();
}
