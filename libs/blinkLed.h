#if !defined(BLINKLED_h)
#define BLINKLED_h

#include <Arduino.h>
#include "hardwareSetup.h"

class blinkLed
{
public:
    unsigned long previousMillis{};
    bool ledState{false};
    blinkLed();
    void update();
    void cleanUp();
} led;

blinkLed::blinkLed()
{
    // setup LED
    pinMode(ledPin, OUTPUT);
}

void blinkLed::update()
{
    unsigned long currentMillis(millis());
    if (currentMillis - previousMillis >= ledFreq)
    {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
    }
}

void blinkLed::cleanUp()
{
    ledState = false;
    digitalWrite(ledPin, ledState);
}

#endif // BLINKLED_h
