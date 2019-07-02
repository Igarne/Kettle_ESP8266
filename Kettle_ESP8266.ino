#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h> 
#include <FS.h>
#include <TickerScheduler.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "DHT.h"


IPAddress ip(192,168,1,202);  //статический IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

String socket_data;
const char* ssid = "Home";
const char* password = "9876543210";

#define dhtPin 12

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
File fsUploadFile;
TickerScheduler ts(2);
Adafruit_BMP085 bmp;
DHT dht;

//format bytes

void setup(void) {
  Serial.begin(115200);
  Serial.print("\n");
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
  
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/list", HTTP_GET, handleFileList);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  BPM_init();
  DHT_init();

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");

}

void loop(void) {
  ts.update(); //планировщик задач
  server.handleClient();
  webSocket.loop();
}
