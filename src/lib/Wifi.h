#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

#define USE_SERIAL Serial
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
static char  buffer[180];  

class Wifi {

  public:
    Wifi();
    void begin(char * string);
    void wifiLoop();
    void wifiWriteLoop();
};
