#include "Led.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "../Var.h"
#include <FastLED.h>

CRGB leds[NUM_LEDS];
CRGB Dots[NUM_DOTS];
bool isBlink = true;

void SetNumber(int segment, int num)
{
    int x = segment * LEDS_PER_SEGMENT - LEDS_PER_SEGMENT;
    int y = 0;

    for (int i = x; i < segment * 7; i++)
    {
        if (Frame[num][y] == 1)
        {
            leds[i] = CRGB(RED, GREEN, BLUE);
        }
        else
        {
            leds[i] = CRGB(0, 0, 0);
        }
        y++;
        FastLED.show();
    }
}

void ErrorDisplay(int one, int two)
{
    SetNumber(1, 11);
    SetNumber(2, 12);
    SetNumber(3, one);
    SetNumber(4, two);
    return;
}

// SEGEMNT FUNCTION
void refreshColor()
{
    RED = EEPROM.read(RED_ADDRESS);
    GREEN = EEPROM.read(GREEN_ADDRESS);
    BLUE = EEPROM.read(BLUE_ADDRESS);
}

void TestStartUp()
{
    Serial.println("");
    Serial.print("Code Version : ");
    Serial.println(CodeVersion);
    refreshColor();
    BlankDisplay(0);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(RED, GREEN, BLUE);
        FastLED.show();
        delay(200);
    }
    BlankDisplay(200);
    return;
}

void BlankDisplay(int gap)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(0, 0, 0);
        FastLED.show();
        delay(gap);
    }
    return;
}

// DOTS FUNCTION
void BlinkDots()
{
    if (isBlink == true)
    {
        ShowDots();
    }
    else
    {
        BlankDots();
    }
    isBlink = !isBlink;
    return;
}

void BlankDots()
{
    for (int i = 0; i < NUM_DOTS; i++)
    {
        Dots[i] = CRGB(0, 0, 0);
    }
    return;
}

void ShowDots()
{
    for (int i = 0; i < NUM_DOTS; i++)
    {
        Dots[i] = CRGB(RED, GREEN, BLUE);
    }
    return;
}
