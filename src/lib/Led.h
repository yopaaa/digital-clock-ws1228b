#ifndef LED
#define LED

#include <FastLED.h>

extern CRGB leds[];
extern CRGB Dots[];

void BlinkDots();
void BlankDots();
void ShowDots();
void ShowDotsRgb(byte Red, byte Green, byte Blue);

void BlankDisplay(int gap);
void BlankSegment(int no);
void TestStartUp();
void refreshColor();
void ErrorDisplay(int one, int two);
void SetNumber(int segment, int num);

void printLocalTime();
void printCounter();
void printCountDown();

#endif