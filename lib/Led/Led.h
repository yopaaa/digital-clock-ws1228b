#ifndef LED
#define LED

#include <Arduino.h>
#include "ADDRESS.h"

extern String CODE_VERSION;
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define NUM_DOTS 2

#if defined(ESP8266)
#define LED_PIN 16
#define DOTS_PIN 14
#define WIFI_AP_BTN 12
#define BUZZER_PIN 13
#elif defined(ESP32)
#define LED_PIN 23
#define DOTS_PIN 22
#define WIFI_AP_BTN 4
#define BUZZER_PIN 21
#endif

// #define for esp32 ship stand alone LED_PIN 26 DOTS_PIN 27 WIFI_AP_BTN 14 BUZZER_PIN 12

extern byte RED;
extern byte GREEN;
extern byte BLUE;
extern byte BRIGHTNESS;

extern bool isPause;

void setupLED();
void TestStartUp();

bool BlinkDots();
void BlankDots();
void ShowDots();
void ShowDotsRgb(byte Red, byte Green, byte Blue);

void BlankDisplay(int gap);
void BlankSegment(int no);
void ErrorDisplay(int one, int two);
void SetNumber(int segment, int num);
void PrintNumber(int num, String segment);
void SetBrightness(byte val);

#endif