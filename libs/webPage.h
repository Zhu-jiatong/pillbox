#if !defined(WEBPAGE_h)
#define WEBPAGE_h

#include <Arduino.h>
#include "myConfig.h"

String webpage{"<head> <title>Pillbox</title></head><body style=\"font-size: 100%; font-size: x-large; font-family: monospace;\"> <table border=\"1\" frame=\"box\" cellpadding=\"10\" style=\"text-align: center; border-collapse: collapse; border: 1px solid darkgray; width: 100%; background-color: beige; font-weight: bolder; font-size: xx-large;\"> <tr> <td>Slot Number</td><td>Alarm Time (hh:mm)</td><td rowspan=\"2\"> <form action=\"/confirm\" method=\"POST\" id=\"setTime\"> <input type=\"submit\" value=\"Submit\" style=\"font-size: xx-large; font-weight: inherit; padding:10%; background-color: forestgreen; font-family: inherit; color: white; border-radius: 10%; border: none;\"/> </form> </td></tr><tr> <td> <select name=\"alarmSlotNo\" style=\"font-size: inherit;\" form=\"setTime\"> <option value=\"0\">Clock (RTC)</option>"};

void constructWebpage()
{
    for (short i = 1; i <= noOfAlarm; ++i)
        webpage += "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
    webpage += "</select> </td><td> <select name=\"alarmH\" style=\"font-size: inherit;\" form=\"setTime\">";
    for (short i = 0; i < 24; ++i)
        webpage += "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
    webpage += "</select> <select name=\"alarmM\" style=\"font-size: inherit;\" form=\"setTime\">";
    for (short i = 0; i < 60; ++i)
        webpage += "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
    webpage += "</select> </td></tr></table></body>";
}

#endif // WEBPAGE_h
