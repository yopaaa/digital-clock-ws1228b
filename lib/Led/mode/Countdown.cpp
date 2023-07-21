#include "Led.h"
#include "Buzzer.h"
#include "Alarm.h"
#include "Mode.h"

unsigned long previousMillisCountDown = 0;

void printCountDown()
{
    unsigned long currentMillis = millis();

    if ((currentMillis - previousMillisCountDown >= interval) && (countDownCount >= 0))
    {
        previousMillisCountDown = currentMillis;

        PrintNumber(countDownCount, "full");
        if (!isPause)
            countDownCount--;
        myBuzzer.off();
    }

    if ((currentMillis - previousMillisCountDown >= (interval / 5)) && (countDownCount <= 0))
    {
        previousMillisCountDown = currentMillis;

        myBuzzer.toggle();
    }
}
