#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <DHT.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);

String readTemp() {
  return String(dht.readTemperature());
}

String readHumi() {
  return String(dht.readHumidity());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  dht.begin();
  
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", readTemp().c_str());
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", readHumi().c_str());
    }); 
    
    // Start server
    server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
