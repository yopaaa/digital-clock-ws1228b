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

        if (!isPause)
            countDownCount--;
        myBuzzer.off();
        PrintNumber(countDownCount, "full");
    }

    if ((currentMillis - previousMillisCountDown >= (interval / 5)) && (countDownCount <= 0))
    {
        previousMillisCountDown = currentMillis;

        myBuzzer.toggle();
    }
}
