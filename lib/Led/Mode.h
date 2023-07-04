#ifndef MODE_H
#define MODE_H

#include <Arduino.h>
#include "ADDRESS.h"

extern String segment1mode;
extern String segment2mode;
extern int interval;

extern String Mode;
extern String colorMode;
extern int counterCount;
extern int counterLimit;
extern int countDownCount;

extern int scors1;
extern int scors2;

extern int timeFormat;

extern String ntpServer;
extern long gmtOffset_sec; // UTC+7
extern int daylightOffset_sec;

void setMode(String mode);
void printCountDown();
void printLocalTime();
void printCounter();
void printScors();
void CLI();

extern void (*mainLoop)();

#endif