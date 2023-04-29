#ifndef LED
#define LED

// #include <FastLED.h>
#include <Arduino.h>

void setupLED();

void BlinkDots();
void BlankDots();
void ShowDots();
void ShowDotsRgb(byte Red, byte Green, byte Blue);

void BlankDisplay(int gap);
void BlankSegment(int no);
void TestStartUp();
void ErrorDisplay(int one, int two);
void SetNumber(int segment, int num);
void PrintNumber(int num, String segment);

void printLocalTime();
void printCounter();
void printCountDown();

#endif