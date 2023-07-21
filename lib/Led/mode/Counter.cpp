#include "Led.h"
#include "Buzzer.h"
#include "Alarm.h"
#include "Mode.h"

unsigned long previousMillisCounter = 0;

void printCounter()
{
    unsigned long currentMillis = millis();

    if ((currentMillis - previousMillisCounter >= interval) && (counterCount <= counterLimit))
    {
        previousMillisCounter = currentMillis;

        PrintNumber(counterCount, "full");
        if (!isPause)
            counterCount++;
        myBuzzer.off();
    }

    if ((currentMillis - previousMillisCounter >= (interval / 5)) && (counterCount >= counterLimit))
    {
        previousMillisCounter = currentMillis;

        myBuzzer.toggle();
    }
}
