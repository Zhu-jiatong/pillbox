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
    String l1{"Prev: " + String(alarm.prevIndx)};
    lcd.setCursor(0, 0);
    lcd.print(l1);

    String l2{"Next: " + String(alarm.minIndx)};
    lcd.setCursor(0, 1);
    lcd.print(l2);

    String l3{alarm.minIndx ? alarm.toStr(alarm.alarmDat[alarm.minIndx].current) : "00:00:00"};
    lcd.setCursor(0, 2);
    lcd.print(l3);

    unsigned long barMax = alarm.minIndx ? (alarm.prevIndx ? alarm.alarmDat[alarm.minIndx].target - alarm.alarmDat[alarm.prevIndx].target : alarm.alarmDat[alarm.minIndx].target) : 0;
    progressBar(3, barMax, alarm.alarmDat[alarm.minIndx].current); // show pregress bar of current alarm at bottom of screen

    lcd.setCursor(11, 0); // r1
    lcd.print(alarm.toStr(alarm.alarmDat[RTCINDX].current));

    String r2{"left: " + String(alarm.noLeft)};
    lcd.setCursor(11, 1);
    lcd.print(r2);

    String r3{"WL: " + String(WiFi.softAPgetStationNum() ? "on  " : "pend")};
    lcd.setCursor(11, 2);
    lcd.print(r3);
}

void display::progressBar(short ln, unsigned long &max, unsigned long &pos)
{
    if (max)
    {
        float factor = max / 100.0;
        unsigned long percent = pos / factor;
        unsigned long number = percent / 5;
        unsigned long remainder = percent % 5;
        if (number)
            for (int j(0); j < number; ++j)
            {
                lcd.setCursor(j, ln);
                lcd.write(5);
            }
        lcd.setCursor(number, ln);
        lcd.write(remainder);
        if (number < 20)
            for (unsigned long j(number + 1); j <= 20; ++j)
            {
                lcd.setCursor(j, ln);
                lcd.write(0);
            }
    }
    else
    {
        lcd.setCursor(0, ln);
        lcd.print("Nice, all cleared :)");
    }
}

#endif // DISPLAY_h
