#include "Led.h"
#include "Var.h"
#include "WifiFunc.h"
#include "EEPROMFunc.h"
#include "FastLED.h"

CRGB leds[NUM_LEDS];
bool isBlink = true;
int ledsLength = NUM_LEDS + NUM_DOTS;

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
    FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, ledsLength);
    FastLED.setBrightness(BRIGHTNESS);
    return;
}

void SetNumber(int segment, int num)
{
    int x = segment * LEDS_PER_SEGMENT - LEDS_PER_SEGMENT;
    int y = 0;
    int oneSideSegment = LEDS_PER_SEGMENT / 7;

    for (int i = x; i < segment * LEDS_PER_SEGMENT; i++)
    {
        bool isOn = Frame[num][int(y / oneSideSegment)] == 1;
        if (isOn)
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
    Serial.println(CODE_VERSION);
    readColor();
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
    for (int i = NUM_LEDS; i < ledsLength; i++)
    {
        leds[i] = CRGB(0, 0, 0);
    }
    FastLED.show();
    return;
}

void ShowDots()
{
    for (int i = NUM_LEDS; i < ledsLength; i++)
    {
        leds[i] = CRGB(RED, GREEN, BLUE);
    }
    FastLED.show();
    return;
}

void ShowDotsRgb(byte Red, byte Green, byte Blue)
{
    for (int i = NUM_LEDS; i < ledsLength; i++)
    {
        leds[i] = CRGB(RED, GREEN, BLUE);
    }
    FastLED.show();
    return;
}
