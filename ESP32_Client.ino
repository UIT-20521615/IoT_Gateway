#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

const char* serverNameTemp = "http://192.168.4.1/temperature";
const char* serverNameHumi = "http://192.168.4.1/humidity";

String temperature;
String humidity;

unsigned long previousMillis = 0;
const long interval = 5000; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
    if(WiFi.status()== WL_CONNECTED ){ 
      temperature = httpGETRequest(serverNameTemp);
      humidity = httpGETRequest(serverNameHumi);
      Serial.println("Temperature: " + temperature + " *C - Humidity: " + humidity + " %");
      previousMillis = currentMillis;
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("Gateway HTTP Response: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  return payload;
}
