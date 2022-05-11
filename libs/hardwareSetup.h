#if !defined(HSETUP_h)
#define HSETUP_h

#include "myConfig.h"
#include "alarmClock.h"
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(lcdAddr, 20, 4); // LCD 1, bridge NONE

#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);

void lcdSetup()
{
    lcd.init();
    lcd.backlight();
    // lcd progress bar chars init
    lcd.createChar(0, zero);
    lcd.createChar(1, one);
    lcd.createChar(2, two);
    lcd.createChar(3, three);
    lcd.createChar(4, four);
    lcd.createChar(5, five);
}

void hardwareInit()
{
#if defined(DEBUG_MODE)

    Serial.begin(9600);

#endif // DEBUG_MODE

    lcdSetup();
    dht.begin();
    attachInterrupt(buttonPin, ackno, RISING);
}

#endif // HSETUP_h
