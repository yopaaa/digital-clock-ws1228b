#include "Led.h"
#include "Mode.h"

unsigned long previousMilliss = 0;

void printScors()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMilliss >= interval)
    {
        previousMilliss = currentMillis;

        BlinkDots();
        PrintNumber(scors1, "left");
        PrintNumber(scors2, "right");
    }
}
