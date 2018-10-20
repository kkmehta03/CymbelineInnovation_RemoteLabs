/*
 * ESP8266 Web server with Web Socket to control an LED.
 *
 * The web server keeps all clients' LED status up to date and any client may
 * turn the LED on or off.
 *
 * For example, clientA connects and turns the LED on. This changes the word
 * "LED" on the web page to the color red. When clientB connects, the word
 * "LED" will be red since the server knows the LED is on.  When clientB turns
 * the LED off, the word LED changes color to black on clientA and clientB web
 * pages.
 *
 * References:
 *
 * https://github.com/Links2004/arduinoWebSockets
 *
 */

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

static const char ssid[] = ".......";
static const char password[] = ".......";
MDNSResponder mdns;
int i;
static void writeLED1(bool);
static void writeLED2(bool);
static void writeLED3(bool);
// GPIO#0 is for Adafruit ESP8266 HUZZAH board. Your board LED might be on 13.

const int LEDPIN[3] = {D3,D2,D1};
// Current LED status
bool LEDStatus[3];




ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>BMSCE PHASE SHIFT REMOTE LAB WORKSHOP</title>
<style>
"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }"
</style>
<script>
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
  websock.onmessage = function(evt) {
    console.log(evt);
    var e = document.getElementById('ledstatus');
    if (evt.data === 'ledon') {
      e.style.color = 'red';
    }
    else if (evt.data === 'ledoff') {
      e.style.color = 'black';
    }
    else {
      console.log('unknown event');
    }
  };
}
function buttonclick(e) {
  websock.send(e.id);
}
</script>
</head>
<body onload="javascript:start();">
<h1>BMSCE PHASE SHIFT REMOTE LAB WORKSHOP</h1>
<div id="ledstatus"><b>IN-A</b></div>
<button id="ledon1"  type="button" onclick="buttonclick(this);">On</button> 
<button id="ledoff1" type="button" onclick="buttonclick(this);">Off</button>
<div id="ledstatus"><b>IN-B</b></div>
<button id="ledon2"  type="button" onclick="buttonclick(this);">On</button> 
<button id="ledoff2" type="button" onclick="buttonclick(this);">Off</button>
<div id="ledstatus"><b>IN-C</b></div>
<button id="ledon3"  type="button" onclick="buttonclick(this);">On</button> 
<button id="ledoff3" type="button" onclick="buttonclick(this);">Off</button>
</body>
</html>
)rawliteral";



// Commands sent through Web Socket
const char LEDON1[] = "ledon1";
const char LEDOFF1[] = "ledoff1";
const char LEDON2[] = "ledon2";
const char LEDOFF2[] = "ledoff2";
const char LEDON3[] = "ledon3";
const char LEDOFF3[] = "ledoff3";


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // Send the current LED status
        if (LEDStatus[0]) {
          webSocket.sendTXT(num, LEDON1, strlen(LEDON1));
        }
        else {
          webSocket.sendTXT(num, LEDOFF1, strlen(LEDOFF1));
        }
         if (LEDStatus[1]) {
          webSocket.sendTXT(num, LEDON2, strlen(LEDON2));
        }
        else {
          webSocket.sendTXT(num, LEDOFF2, strlen(LEDOFF2));
        }
         if (LEDStatus[0]) {
          webSocket.sendTXT(num, LEDON3, strlen(LEDON3));
        }
        else {
          webSocket.sendTXT(num, LEDOFF3, strlen(LEDOFF3));
        }
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);

      if (strcmp(LEDON1, (const char *)payload) == 0) {
        writeLED1(true);
      }
      else if (strcmp(LEDOFF1, (const char *)payload) == 0) {
        writeLED1(false);
      }
       if (strcmp(LEDON2, (const char *)payload) == 0) {
        writeLED2(true);
      }
      else if (strcmp(LEDOFF2, (const char *)payload) == 0) {
        writeLED2(false);
      }
       if (strcmp(LEDON3, (const char *)payload) == 0) {
        writeLED3(true);
      }
      else if (strcmp(LEDOFF3, (const char *)payload) == 0) {
        writeLED3(false);
      }
      else {
        Serial.println("Unknown command");
      }
      // send data to all connected clients
      webSocket.broadcastTXT(payload, length);
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}

void handleRoot()
{
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

static void writeLED1(bool LEDon1)
{
  LEDStatus[0] = LEDon1;
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon1) {
    digitalWrite(LEDPIN[0], 1); 
  }
  else {
    digitalWrite(LEDPIN[0], 0);
  }
}
static void writeLED2(bool LEDon2)
{
  LEDStatus[1] = LEDon2;
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon2) {
    digitalWrite(LEDPIN[1], 1); 
  }
  else {
    digitalWrite(LEDPIN[1], 0);
  }
}

static void writeLED3(bool LEDon3)
{
  LEDStatus[2] = LEDon3;
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon3) {
    digitalWrite(LEDPIN[2], 1); 
  }
  else {
    digitalWrite(LEDPIN[2], 0);
  }
}


void setup()
{
IPAddress ip(192,168,43,207);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);
WiFi.config(ip, gateway, subnet);
WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  for(i=0;i<3;i++)
  {
  pinMode(LEDPIN[i], OUTPUT);
  }
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    pinMode(D0, OUTPUT);
for(i=0;i<3;i++)
  {
  digitalWrite(LEDPIN[i], LOW);
  }

    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);
    digitalWrite(D0, LOW);

  Serial.begin(115200);

  //Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

//  WiFiMulti.addAP(ssid, password);

  while(WiFi.status()!= WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("asee1", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("Connect to http://ASEE1.local or http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
  bool a,b,c, xorG, orG, andG, nandG, faS, faC;
  a = LEDStatus[0];
  b = LEDStatus[1];
  c = LEDStatus[2];
  xorG = a xor b;
  orG = a or b;
  andG = a and b;
  nandG= !andG;
  faS = xorG xor c;
  faC = andG or (c and xorG);
  digitalWrite(D0, nandG);
  digitalWrite(D5, orG);
  digitalWrite(D6, xorG);
  digitalWrite(D7, andG);
  digitalWrite(D8, faS);
  //digitalWrite(D0, faC);
  delay(10);
}
