#include "Arduino.h"
#include "Wifi.h"
#include "BufferSerial.h"
#define NETWORKNAME "TBPM"
#define NETWORKPASS "donger420"

#include "PString.h"

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(8899);

Wifi::Wifi(){

}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] Message: %s\n", num, payload);
            //webSocket.sendTXT(num,payload);
            //_s->printf("%s",payload);
            break;
    }

}

void Wifi::wifiWriteLoop() {
  BufferSerial * x; 

  x->begin(1);
  //stream.read()
  //char buffer[180];
  //PString mystring(buffer, sizeof(buffer));
  //mystring.print("Hi, my name is ");
  //mystring.print(" Two");

 // mystring.flush();
  //mystring.begin();
//  mystring.print("sdfg");



 //  webSocket.sendTXT(0,buffer);    
  /*sprintf (buff, "%d plus %d is %d", 5, 5, 5+5);
  if (_s->available() > 0){
    webSocket.sendTXT(0,_s->readString());
    _s->flush();
  }*/
}

void Wifi::begin(Stream &s) {
    _s = &s;
    //USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    int n = WiFi.scanNetworks();
    Serial.println("[SETUP] SCANNING...");
    Serial.println("");
    if (n == 0)
      USE_SERIAL.println("no networks found");
    else
    {
      USE_SERIAL.print(n);
      USE_SERIAL.println(" networks found");
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        USE_SERIAL.print(i + 1);
        USE_SERIAL.print(": ");
        USE_SERIAL.print(WiFi.SSID(i));
        USE_SERIAL.print(" (");
        USE_SERIAL.print(WiFi.RSSI(i));
        USE_SERIAL.print(")");
        USE_SERIAL.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(10);
      }
    }
    Serial.println("");

    WiFiMulti.addAP(NETWORKNAME, NETWORKPASS);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    USE_SERIAL.print("IP address: ");
    USE_SERIAL.println(WiFi.localIP());
    USE_SERIAL.flush();

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if(MDNS.begin("esp8266")) {
        USE_SERIAL.println("MDNS responder started");
    }

    // handle index
    server.on("/", []() {
        // send index.html
        server.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':8899/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendRGB() {  var r = parseInt(document.getElementById('r').value).toString(16);  var g = parseInt(document.getElementById('g').value).toString(16);  var b = parseInt(document.getElementById('b').value).toString(16);  if(r.length < 2) { r = '0' + r; }   if(g.length < 2) { g = '0' + g; }   if(b.length < 2) { b = '0' + b; }   var rgb = '#'+r+g+b;    console.log('RGB: ' + rgb); connection.send(rgb); }</script></head><body>LED Control:<br/><br/>R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"sendRGB();\" /><br/>G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"sendRGB();\" /><br/>B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"sendRGB();\" /><br/></body></html>");
    });

    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 8899);

}

void Wifi::wifiLoop() {
    webSocket.loop();
    server.handleClient();
}
