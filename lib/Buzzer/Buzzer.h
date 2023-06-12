#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer
{
private:
    int pin;

public:
    Buzzer(int pin);
    void on();
    void off();
    void beep(int timeout);
};

extern Buzzer myBuzzer;

#endif