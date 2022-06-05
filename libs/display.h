#if !defined(DISPLAY_h)
#define DISPLAY_h

#include <Arduino.h>
#include "hardwareSetup.h"
#include "alarmClock.h"
#include "espConfig.h"

class display
{
public:
    void refresh();
    void progressBar(short ln, unsigned long &max, unsigned long &pos);
} screen;

void display::refresh()
{
    lcd.setCursor(0, 0);
    lcd.print("Prev: " + String(alarm.prevIndx));

    lcd.setCursor(0, 1);
    lcd.print("Next: " + String(alarm.minIndx));

    lcd.setCursor(0, 2);
    lcd.print(alarm.minIndx ? alarm.toStr(alarm.alarmDat[alarm.minIndx].current) : "00:00:00");

    unsigned long barMax = alarm.minIndx ? (alarm.prevIndx ? alarm.alarmDat[alarm.minIndx].target - alarm.alarmDat[alarm.prevIndx].target : alarm.alarmDat[alarm.minIndx].target) : 0;
    progressBar(3, barMax, alarm.alarmDat[alarm.minIndx].current); // show pregress bar of current alarm at bottom of screen

    lcd.setCursor(11, 0); // r1
    lcd.print(alarm.toStr(alarm.alarmDat[RTCINDX].current));

    lcd.setCursor(11, 1);
    lcd.print("left: " + String(alarm.noLeft));

    lcd.setCursor(11, 2);
    lcd.print("WL: " + String(WiFi.softAPgetStationNum() ? "on  " : "idle"));
}

void display::progressBar(short ln, unsigned long &max, unsigned long &pos)
{
    lcd.setCursor(0, ln);
    if (max)
    {
        short percent = pos / (max / 100.0);
        short number = percent / 5;
        if (number)
            for (short i(0); i < number; ++i)
                lcd.write(5);
        lcd.write(percent % 5);
        if (number < 20)
            for (short i(number + 1); i < 20; ++i)
                lcd.write(0);
    }
    else
        lcd.print("Nice, all cleared :)");
}

#endif // DISPLAY_h
