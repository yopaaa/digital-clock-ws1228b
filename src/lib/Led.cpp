#include "Led.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "../Var.h"
#include <FastLED.h>

CRGB leds[NUM_LEDS];
CRGB Dots[NUM_DOTS];
bool isBlink = true;

const int Frame[][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // Number 0
    {0, 0, 1, 1, 0, 0, 0}, // Number 1
    {0, 1, 1, 0, 1, 1, 1}, // Number 2
    {0, 1, 1, 1, 1, 0, 1}, // Number 3
    {1, 0, 1, 1, 0, 0, 1}, // Number 4
    {1, 1, 0, 1, 1, 0, 1}, // Number 5
    {1, 1, 0, 1, 1, 1, 1}, // Number 6
    {0, 1, 1, 1, 0, 0, 0}, // Number 7
    {1, 1, 1, 1, 1, 1, 1}, // Number 8
    {1, 1, 1, 1, 1, 0, 1}, // Number 9
    {0, 0, 0, 0, 0, 0, 0}, // Digit off
    {1, 1, 0, 0, 1, 1, 1}, // Alfabet E
    {1, 1, 0, 0, 0, 1, 0}, // Alfabet r
    {0, 0, 0, 0, 1, 0, 0}, // Simbol _
};

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

void BlankSegment(int no)
{
    SetNumber(no, 10);
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

void ShowDotsRgb(byte Red, byte Green, byte Blue)
{
    for (int i = 0; i < NUM_DOTS; i++)
    {
        Dots[i] = CRGB(Red, Green, Blue);
    }
    return;
}

void printLocalTime()
{
    BlinkDots();
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        ErrorDisplay(0, 2);
        return;
    }
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    // int second = timeinfo.tm_sec;
    int minute = timeinfo.tm_min;
    int hour = timeinfo.tm_hour;
    if ((timeFormat == 12) && (hour > 12))
        hour = hour - 12;

    if (ColorMode == "random")
    {
        RED = random(1, 255);
        GREEN = random(1, 255);
        BLUE = random(1, 255);
    }

    // Print hour
    if (hour < 10)
    {
        BlankSegment(1);
        SetNumber(2, hour);
    }
    else
    {
        SetNumber(1, hour / 10);
        SetNumber(2, hour % 10);
    }

    // Print minute
    if (minute < 10)
    {
        BlankSegment(4);
        SetNumber(3, minute);
    }
    else
    {
        SetNumber(3, minute / 10);
        SetNumber(4, minute % 10);
    }
    // SetNumber(3, second / 10);
    // SetNumber(4, second % 10);
}

void printCounter()
{
    if (counterCount <= counterLimit)
    {
        if (counterCount < 10)
        { // 0-9
            BlankDisplay(0);
            SetNumber(4, counterCount);
        }
        else if ((counterCount > 9) && (counterCount < 100))
        { // 9-99
            int firstDigit = counterCount / 10;
            int secondDigit = counterCount % 10;

            SetNumber(3, firstDigit);
            SetNumber(4, secondDigit);
        }
        else if ((counterCount > 99) && (counterCount < 1000))
        { // 99-999
            int firstDigit = counterCount / 100;
            int secondDigit = (counterCount % 100) / 10;
            int thirdDigit = counterCount % 10;

            SetNumber(2, firstDigit);
            SetNumber(3, secondDigit);
            SetNumber(4, thirdDigit);
        }
        else if ((counterCount > 999) && (counterCount < 10000))
        { // 999-9999
            int firstDigit = counterCount / 1000;
            int secondDigit = (counterCount % 1000) / 100;
            int thirdDigit = (counterCount % 100) / 10;
            int fourthDigit = counterCount % 10;

            SetNumber(1, firstDigit);
            SetNumber(2, secondDigit);
            SetNumber(3, thirdDigit);
            SetNumber(4, fourthDigit);
        }
        counterCount++;
        return;
    }
    return;
}

void printCountDown()
{
    if (countDownCount >= 0)
    {
        if (countDownCount < 10)
        { // 0-9
            SetNumber(4, countDownCount);
        }
        else if ((countDownCount > 9) && (countDownCount < 100))
        { // 9-99
            int firstDigit = countDownCount / 10;
            int secondDigit = countDownCount % 10;

            SetNumber(3, firstDigit);
            SetNumber(4, secondDigit);
        }
        else if ((countDownCount > 99) && (countDownCount < 1000))
        { // 99-999
            int firstDigit = countDownCount / 100;
            int secondDigit = (countDownCount % 100) / 10;
            int thirdDigit = countDownCount % 10;

            SetNumber(2, firstDigit);
            SetNumber(3, secondDigit);
            SetNumber(4, thirdDigit);
        }
        else if ((countDownCount > 999) && (countDownCount < 10000))
        { // 999-9999
            int firstDigit = countDownCount / 1000;
            int secondDigit = (countDownCount % 1000) / 100;
            int thirdDigit = (countDownCount % 100) / 10;
            int fourthDigit = countDownCount % 10;

            SetNumber(1, firstDigit);
            SetNumber(2, secondDigit);
            SetNumber(3, thirdDigit);
            SetNumber(4, fourthDigit);
        }
        BlankDisplay(0);
        countDownCount--;
        return;
    }
    return;
}
