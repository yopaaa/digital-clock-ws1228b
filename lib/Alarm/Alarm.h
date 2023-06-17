#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

#define MAX_ALARMS 5

struct Alarm
{
    int hour;
    int min;
    int days[7];
    int alertIndex;
};

// array to store alarm
extern Alarm alarms[MAX_ALARMS];
extern int numAlarms;
void readAlarmsFromEEPROM();
void writeAlarmsToEEPROM();
void checkAlarm(int day, int hour, int minute);
void setAlarm(int index, int days[7], int hour, int min, int alertIndex);
void playAlarm(int gap);

void hexToBinaryArray(unsigned int hexValue, int *binaryArray);
unsigned int binaryArrayToHex(const int *binaryArray);
#endif