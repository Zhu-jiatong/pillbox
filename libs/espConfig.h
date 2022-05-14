#if !defined(ESPCONFIG_h)
#define ESPCONFIG_h

#include <Arduino.h>
#include "alarmClock.h"
#include "myConfig.h"
#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;

#include <ESP8266WiFi.h>
// IPAddress apIP(192, 168, 1, 1);

#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
void handleRoot();           // function prototypes for HTTP handlers
void handleNotFound();
void handleSetTime();

void handleRoot()
{
  server.send(200, "text/html", "<head> <title>Pillbox</title></head><body style=\"font-size: 100%; font-size: x-large; font-family: monospace;\"> <table border=\"1\" frame=\"box\" cellpadding=\"10\" style=\"text-align: center; border-collapse: collapse; border: 1px solid darkgray; width: 100%; background-color: beige;\"> <tr style=\"font-weight: bolder; font-size: xx-large;\"> <td>slot number</td><td>alarm time - hh:mm</td><td rowspan=\"2\"> <form action=\"/confirm\" method=\"POST\" id=\"setTime\"> <input type=\"submit\" value=\"Submit\" style=\"font-size: xx-large; font-weight: inherit; padding:10%; background-color: forestgreen; font-family: inherit; color: white; border-radius: 10%; border: none;\"/> </form> </td></tr><tr> <td> <select name=\"alarmSlotNo\" style=\"font-size: inherit;\" form=\"setTime\"> <option value=\"0\">Clock (RTC)</option> <option value=\"1\">1</option> <option value=\"2\">2</option> <option value=\"3\">3</option> <option value=\"4\">4</option> <option value=\"5\">5</option> <option value=\"6\">6</option> <option value=\"7\">7</option> <option value=\"8\">8</option> </select> </td><td> <select name=\"alarmH\" style=\"font-size: inherit;\" form=\"setTime\"> <option value=\"0\">0</option> <option value=\"1\">1</option> <option value=\"2\">2</option> <option value=\"3\">3</option> <option value=\"4\">4</option> <option value=\"5\">5</option> <option value=\"6\">6</option> <option value=\"7\">7</option> <option value=\"8\">8</option> <option value=\"9\">9</option> <option value=\"10\">10</option> <option value=\"11\">11</option> <option value=\"12\">12</option> <option value=\"13\">13</option> <option value=\"14\">14</option> <option value=\"15\">15</option> <option value=\"16\">16</option> <option value=\"17\">17</option> <option value=\"18\">18</option> <option value=\"19\">19</option> <option value=\"20\">20</option> <option value=\"21\">21</option> <option value=\"22\">22</option> <option value=\"23\">23</option> </select> <b>:</b> <select name=\"alarmM\" style=\"font-size: inherit;\" form=\"setTime\"> <option value=\"0\">0</option> <option value=\"1\">1</option> <option value=\"2\">2</option> <option value=\"3\">3</option> <option value=\"4\">4</option> <option value=\"5\">5</option> <option value=\"6\">6</option> <option value=\"7\">7</option> <option value=\"8\">8</option> <option value=\"9\">9</option> <option value=\"10\">10</option> <option value=\"11\">11</option> <option value=\"12\">12</option> <option value=\"13\">13</option> <option value=\"14\">14</option> <option value=\"15\">15</option> <option value=\"16\">16</option> <option value=\"17\">17</option> <option value=\"18\">18</option> <option value=\"19\">19</option> <option value=\"20\">20</option> <option value=\"21\">21</option> <option value=\"22\">22</option> <option value=\"23\">23</option> <option value=\"24\">24</option> <option value=\"25\">25</option> <option value=\"26\">26</option> <option value=\"27\">27</option> <option value=\"28\">28</option> <option value=\"29\">29</option> <option value=\"30\">30</option> <option value=\"31\">31</option> <option value=\"32\">32</option> <option value=\"33\">33</option> <option value=\"34\">34</option> <option value=\"35\">35</option> <option value=\"36\">36</option> <option value=\"37\">37</option> <option value=\"38\">38</option> <option value=\"39\">39</option> <option value=\"40\">40</option> <option value=\"41\">41</option> <option value=\"42\">42</option> <option value=\"43\">43</option> <option value=\"44\">44</option> <option value=\"45\">45</option> <option value=\"46\">46</option> <option value=\"47\">47</option> <option value=\"48\">48</option> <option value=\"49\">49</option> <option value=\"50\">50</option> <option value=\"51\">51</option> <option value=\"52\">52</option> <option value=\"53\">53</option> <option value=\"54\">54</option> <option value=\"55\">55</option> <option value=\"56\">56</option> <option value=\"57\">57</option> <option value=\"58\">58</option> <option value=\"59\">59</option> </select> </td></tr></table></body>"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}
void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
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
  //WiFi.mode(WIFI_AP);
  // WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.pillbox.com", WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot); // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/confirm", HTTP_POST, handleSetTime);
  server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                    // Actually start the server
}

#endif // ESPCONFIG_h
