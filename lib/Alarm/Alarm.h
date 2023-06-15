#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

#define MAX_ALARMS 5

struct Alarm
{
    int hour;
    int min;
    int alarmDelay;
};

// array to store alarm
extern Alarm alarms[MAX_ALARMS];
extern int numAlarms;
void readAlarmsFromEEPROM();
void writeAlarmsToEEPROM();
void checkAlarm(int hour, int minute);
void setAlarm(int index, int hour, int min, int alarmDelay);
void playAlarm(int gap);
#endif