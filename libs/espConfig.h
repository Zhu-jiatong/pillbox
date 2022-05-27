#if !defined(ESPCONFIG_h)
#define ESPCONFIG_h

#include <Arduino.h>
#include "alarmClock.h"
#include "myConfig.h"
#include "webPage.h"
#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
void handleRoot();           // function prototypes for HTTP handlers
void handleSetTime();

void handleRoot()
{
  server.send(200, "text/html", webpage); // Send HTTP status 200 (Ok) and send some text to the browser/client
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

void startWifi()
{
  WiFi.softAP(ssid);

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
