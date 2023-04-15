#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>

// Set up pins for flashlight and speaker
int flashlightPin = D3;
int speakerPin = D5;

// Set up variables for Wi-Fi network
char ssid[30] = "myWifiNetwork";  // Change to your network name
char password[30] = "myWifiPassword";  // Change to your network password
boolean shouldSaveConfig = false;
WiFiManager wifiManager;

// Set up web server for changing Wi-Fi password
ESP8266WebServer server(80);

void setup() {
  // Set up pins for flashlight and speaker
  pinMode(flashlightPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // Connect to Wi-Fi network
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect("ESP8266AP");
  if (shouldSaveConfig) {
    saveConfig();
  }

  // Set up web server for changing Wi-Fi password
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();

  // Start ShakeAlert connection
  HTTPClient http;
  http.begin("https://api.shakealert.org/alerts/");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    // Parse earthquake data from ShakeAlert and trigger warning
    // code here
  }
  http.end();
}

void loop() {
  // Check if connected to Wi-Fi network
  if (WiFi.status() == WL_CONNECTED) {
    // Turn off broadcasting wifi
    WiFi.mode(WIFI_STA);
  } else {
    // Turn on broadcasting wifi
    WiFi.mode(WIFI_AP);
  }

  // Handle any incoming web server requests
  server.handleClient();
}

void handleRoot() {
  // Serve web page for changing Wi-Fi password
  // code here
}

void handleSubmit() {
  // Save new Wi-Fi password and name
  // code here
}

void saveConfigCallback() {
  shouldSaveConfig = true;
}

void saveConfig() {
  wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 0, 99), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
  strcpy(ssid, wifiManager.getSSID().c_str());
  strcpy(password, wifiManager.getPassword().c_str());
}
