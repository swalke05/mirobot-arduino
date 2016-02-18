#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

#define USE_SERIAL Serial
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

class Wifi {
  public:
    Wifi();
    void begin(Stream &s);
    void wifiLoop();
    void wifiWriteLoop();
  private:
    Stream *_s;
};
