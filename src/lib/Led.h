#ifndef LED
#define LED

#include <FastLED.h>

extern CRGB leds[];
extern CRGB Dots[];

void BlinkDots();
void BlankDots();
void ShowDots();
void BlankDisplay(int gap);
void TestStartUp();
void refreshColor();
void ErrorDisplay(int one, int two);
void SetNumber(int segment, int num);

#endif