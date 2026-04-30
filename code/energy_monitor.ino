#include <WiFi.h>
#include <HTTPClient.h>
#include <PZEM004Tv30.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ThingSpeak API
String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

// Create PZEM object (RX, TX pins of ESP32)
PZEM004Tv30 pzem(Serial1, 16, 17);

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();

  if (!isnan(voltage)) {
    Serial.println("Voltage: " + String(voltage) + " V");
    Serial.println("Current: " + String(current) + " A");
    Serial.println("Power: " + String(power) + " W");
    Serial.println("Energy: " + String(energy) + " kWh");
  } else {
    Serial.println("Error reading data");
  }

  // Send Data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(voltage) +
                 "&field2=" + String(current) +
                 "&field3=" + String(power) +
                 "&field4=" + String(energy);

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.println("Data sent to ThingSpeak");
  }

  delay(15000);  // ThingSpeak minimum delay
}
