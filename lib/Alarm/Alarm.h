#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

#define MAX_ALARMS 5 // maximum number of alarms that can be stored.

struct Alarm
{
    int hour;       // int 1 to 24.
    int min;        // int 0 to 59.
    int days[7];    // {1,1,1,1,1,1,1} equal {Saturday, Friday, Thursday, Wednesday, Tuesday, Monday, Sunday}.
    int alertIndex; // alert index to alarm.
};

extern Alarm alarms[MAX_ALARMS];                                          // array to store alarm.
void readAlarmsFromEEPROM();                                              // function to read saved alarm from eeprom.
void writeAlarmsToEEPROM();                                               // function to write/saved alarm to eeprom.
void checkAlarm(int day, int hour, int minute);                           // function to check if there is an alarm at the current time.
void setAlarm(int index, int days[7], int hour, int min, int alertIndex); // function to update or create a new alarm.
void playAlarm(int gap);                                                  // function to play alarm

void hexToBinaryArray(unsigned int hexValue, int *binaryArray); // function to convert hexadecimal to binary array like 62 > {0,1,1,1,1,1,0}
unsigned int binaryArrayToHex(const int *binaryArray);          // function to convert binary array to hexadecimal like {0,1,1,1,1,1,0} > 62

#endif
