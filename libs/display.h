#if !defined(DISPLAY_h)
#define DISPLAY_h

#include <Arduino.h>
#include "hardwareSetup.h"
#include "alarmClock.h"

class display
{
public:
    String l1, l2, l3, r1, r2, r3;
    void refresh();
    void progressBar(short ln, unsigned long max, unsigned long pos);
} screen;

void display::refresh()
{
    l1 = "Prev: " + String(alarm.prevIndx);
    lcd.setCursor(0, 0);
    lcd.print(l1);

    l2 = "Next: " + String(alarm.minIndx);
    lcd.setCursor(0, 1);
    lcd.print(l2);

    l3 = alarm.minIndx ? alarm.toStr(alarm.alarmDat[alarm.minIndx].current) : "0";
    lcd.setCursor(0, 2);
    lcd.print(l3);

    unsigned long barMax = alarm.minIndx ? (alarm.prevIndx ? alarm.alarmDat[alarm.minIndx].target - alarm.alarmDat[alarm.prevIndx].target : alarm.alarmDat[alarm.minIndx].target) : 0;
    progressBar(3, barMax, alarm.alarmDat[alarm.minIndx].current); // show pregress bar of current alarm at bottom of screen

    lcd.setCursor(11, 0); // r1
    lcd.print(alarm.toStr(alarm.alarmDat[RTCINDX].current));

    r2 = "T: " + String(dht.readTemperature()) + "C";
    lcd.setCursor(11, 1);
    lcd.print(r2);

    r3 = "H: " + String(dht.readHumidity()) + "%";
    lcd.setCursor(11, 2);
    lcd.print(r3);
}

void display::progressBar(short ln, unsigned long max, unsigned long pos)
{
    if (max)
    {
        double factor = max / 100.0;
        unsigned long percent = pos / factor;
        unsigned long number = percent / 5;
        unsigned long remainder = percent % 5;
        if (number > 0)
        {
            for (int j(0); j < number; ++j)
            {
                lcd.setCursor(j, ln);
                lcd.write(5);
            }
        }
        lcd.setCursor(number, ln);
        lcd.write(remainder);
        if (number < 20)
        {
            for (unsigned long j(number + 1); j <= 20; ++j)
            {
                lcd.setCursor(j, ln);
                lcd.write(0);
            }
        }
    }
    else
    {
        lcd.setCursor(0, ln);
        lcd.print("Nice, all done");
    }
}

#endif // DISPLAY_h
