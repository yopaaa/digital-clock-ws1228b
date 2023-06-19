#include "Led.h"
#include "Buzzer.h"
#include "Alarm.h"
#include "Mode.h"

void printCountDown()
{
    if (countDownCount >= 0)
    {
        PrintNumber(countDownCount, "full");
        countDownCount--;
        myBuzzer.off();
        delay(interval);
        return;
    }
    else
    {
        playAlarm(200);
    }
}

