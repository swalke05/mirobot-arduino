#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

#define USE_SERIAL Serial

void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);

class Wifi {
  public:
    Wifi();
    void begin();
    void wifiLoop();
  private:
};
