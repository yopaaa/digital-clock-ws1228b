#include "Led.h"
#include "../Var.h"
#include "WifiFunc.h"
#include "EEPROMFunc.h"

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

void setupLED()
{
    FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<LED_TYPE, DOTS_PIN, GRB>(Dots, NUM_DOTS);
    FastLED.setBrightness(BRIGHTNESS);
    return;
}

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

void PrintNumber(int num, String segment)
{
    if (segment == "full")
    {
        BlankDisplay(1);
        if (num < 10) // 0-9
        {
            SetNumber(4, num);
        }
        else if ((num > 9) && (num < 100)) // 9-99
        {
            int firstDigit = num / 10;
            int secondDigit = num % 10;

            SetNumber(3, firstDigit);
            SetNumber(4, secondDigit);
        }
        else if ((num > 99) && (num < 1000)) // 99-999
        {
            int firstDigit = num / 100;
            int secondDigit = (num % 100) / 10;
            int thirdDigit = num % 10;

            SetNumber(2, firstDigit);
            SetNumber(3, secondDigit);
            SetNumber(4, thirdDigit);
        }
        else if ((num > 999) && (num < 10000)) // 999-9999
        {
            int firstDigit = num / 1000;
            int secondDigit = (num % 1000) / 100;
            int thirdDigit = (num % 100) / 10;
            int fourthDigit = num % 10;

            SetNumber(1, firstDigit);
            SetNumber(2, secondDigit);
            SetNumber(3, thirdDigit);
            SetNumber(4, fourthDigit);
        }
    }
    else if (segment == "right")
    {
        if (num < 10)
        {
            BlankSegment(3);
            SetNumber(4, num);
        }
        else
        {
            SetNumber(3, num / 10);
            SetNumber(4, num % 10);
        }
    }
    else if (segment == "left")
    {
        if (num < 10)
        {
            BlankSegment(1);
            SetNumber(2, num);
        }
        else
        {
            SetNumber(1, num / 10);
            SetNumber(2, num % 10);
        }
    }
    return;
}

void ErrorDisplay(int one, int two)
{
    SetNumber(1, 11);
    SetNumber(2, 12);
    SetNumber(3, one);
    SetNumber(4, two);
    return;
}

void TestStartUp()
{
    Serial.println("");
    Serial.print("Code Version : ");
    Serial.println(CodeVersion);
    readColor();
    BlankDisplay(0);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        int buttonState = digitalRead(WIFI_AP_BTN);
        if (buttonState == LOW)
        {
            startWifiAp();
        }
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
    FastLED.show();
    return;
}

void ShowDots()
{
    for (int i = 0; i < NUM_DOTS; i++)
    {
        Dots[i] = CRGB(RED, GREEN, BLUE);
    }
    FastLED.show();
    return;
}

void ShowDotsRgb(byte Red, byte Green, byte Blue)
{
    for (int i = 0; i < NUM_DOTS; i++)
    {
        Dots[i] = CRGB(Red, Green, Blue);
    }
    FastLED.show();
    return;
}

void printLocalTime()
{
    if (ColorMode == "random")
    {
        RED = random(1, 255);
        GREEN = random(1, 255);
        BLUE = random(1, 255);
    }
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

    PrintNumber(hour, "left");
    PrintNumber(minute, "right");
    // SetNumber(3, second / 10);
    // SetNumber(4, second % 10);
}

void printCounter()
{
    if (counterCount <= counterLimit)
    {
        PrintNumber(counterCount, "full");
        counterCount++;
        return;
    }
    return;
}

void printCountDown()
{
    if (countDownCount >= 0)
    {
        PrintNumber(countDownCount, "full");
        countDownCount--;
        return;
    }
    return;
}
