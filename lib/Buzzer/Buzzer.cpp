#include "Buzzer.h"

Buzzer myBuzzer(BUZZER_PIN);

Buzzer::Buzzer(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Buzzer::on()
{
    digitalWrite(pin, HIGH);
}

void Buzzer::off()
{
    digitalWrite(pin, LOW);
}

void Buzzer::beep(int timeout)
{
    digitalWrite(pin, HIGH);
    delay(timeout);
    digitalWrite(pin, LOW);
}

