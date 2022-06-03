#if !defined(ESPCONFIG_h)
#define ESPCONFIG_h

#include <Arduino.h>
#include "alarmClock.h"
#include "myConfig.h"
#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
String webpageCfg{"<head> <title>Pillbox</title></head><body style=\"font-size: 100%; font-size: x-large; font-family: monospace;\"> <table border=\"1\" frame=\"box\" cellpadding=\"10\" style=\"text-align: center; border-collapse: collapse; border: 1px solid darkgray; width: 100%; background-color: beige; font-weight: bolder; font-size: xx-large;\"> <tr> <td>Slot Number</td><td>Alarm Time (hh:mm)</td><td rowspan=\"2\"> <form action=\"/confirm\" method=\"POST\" id=\"setTime\"> <input type=\"submit\" value=\"Submit\" style=\"font-size: xx-large; font-weight: inherit; padding:10%; background-color: forestgreen; font-family: inherit; color: white; border-radius: 10%; border: none;\"/> </form> </td></tr><tr> <td> <select name=\"alarmSlotNo\" style=\"font-size: inherit;\" form=\"setTime\"> <option value=\"0\">Clock (RTC)</option>"};
String webpageInfo{"<br><table border=\"1\" frame=\"box\" cellpadding=\"10\" style=\"text-align: center; border-collapse: collapse; border: 1px solid darkgray; width: 100%; background-color: beige; font-weight: bolder; font-size: xx-large;\"><tr><td>Slot Number</td><td>Alarm Time (hh:mm:ss)</td><td>Status</td></tr>"};
String constructWebpageInfo();
void handleRoot(); // function prototypes for HTTP handlers
void handleSetTime();

void handleRoot()
{
  server.send(200, "text/html", webpageCfg + constructWebpageInfo()); // Send HTTP status 200 (Ok) and send some text to the browser/client
  server.send(200, "text/html", "</body>");
}
void handleSetTime()
{
#if defined(DEBUG_MODE)
  Serial.println(server.arg("alarmSlotNo").toInt());
  Serial.println(server.arg("alarmH").toInt());
  Serial.println(server.arg("alarmM").toInt());
#endif // DEBUG

  alarm.set(server.arg("alarmSlotNo").toInt(), server.arg("alarmH").toInt(), server.arg("alarmM").toInt());
  handleRoot();
}

String constructWebpageInfo()
{
  String temp{webpageInfo};
  for (short i = 1; i <= noOfAlarm; ++i)
    temp += "<tr> <td>" + String(i) + "</td><td>" + alarm.toStr(alarm.alarmDat[i].target) + "</td><td>" + String(alarm.alarmDat[i].state == alarm.RUN ? "Active" : alarm.alarmDat[i].state == alarm.EXPI ? "Expired"
                                                                                                                                                               : alarm.alarmDat[i].state == alarm.IDLE   ? "Idle"
                                                                                                                                                                                                         : "Acknowledged") +
            "</td></tr>";
  temp += "</table></body>";
  return temp;
}

void constructWebpageCfg()
{
  for (short i = 1; i <= noOfAlarm; ++i)
    webpageCfg += "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
  webpageCfg += "</select> </td><td> <select name=\"alarmH\" style=\"font-size: inherit;\" form=\"setTime\">";
  for (short i = 0; i < 24; ++i)
    webpageCfg += "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
  webpageCfg += "</select> <select name=\"alarmM\" style=\"font-size: inherit;\" form=\"setTime\">";
  for (short i = 0; i < 60; ++i)
    webpageCfg += "<option value=\"" + String(i) + "\">" + String(i) + "</option>";
  webpageCfg += "</select> </td></tr></table>";
}

void startWifi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pwd, 1, 0, 1);

  constructWebpageCfg();

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.pillbox.com", WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot); // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/confirm", HTTP_POST, handleSetTime);
  server.onNotFound([]()
                    { server.send(404, "text/plain", "404: Not found"); }); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                                                           // Actually start the server
}

#endif // ESPCONFIG_h
