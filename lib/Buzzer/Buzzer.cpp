#include "Buzzer.h"
#include "Var.h"

Buzzer myBuzzer(BUZZER_PIN);

Buzzer::Buzzer(int pin)
{
    this->pin = pin;
}

void Buzzer::on()
{
    digitalWrite(pin, HIGH);
}

void Buzzer::off()
{
    digitalWrite(pin, LOW);
}

void Buzzer::toggle()
{
    digitalWrite(pin, !digitalRead(pin));
}

void Buzzer::beep(int timeout)
{
    digitalWrite(pin, HIGH);
    delay(timeout);
    digitalWrite(pin, LOW);
}

