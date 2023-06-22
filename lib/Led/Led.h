#ifndef LED
#define LED

#include <Arduino.h>
#include "ADDRESS.h"

#define CODE_VERSION "v3.6.21" // tahun 202'3', bulan .., tanggal ..
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define NUM_DOTS 2

#if defined(ESP8266)
#define LED_PIN 0
#define WIFI_AP_BTN 4
#define BUZZER_PIN 2
#elif defined(ESP32)
#define LED_PIN 23
#define WIFI_AP_BTN 4
#define BUZZER_PIN 22
#endif

extern byte RED;
extern byte GREEN;
extern byte BLUE;
extern byte BRIGHTNESS;


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

void setMode(String mode);

#endif