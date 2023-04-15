#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SPEAKER_PIN D5
#define FLASHLIGHT_PIN D3
#define GPS_RX_PIN D7
#define GPS_TX_PIN D8
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(FLASHLIGHT_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.begin();
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      Serial.print("Location: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(", ");
      Serial.println(gps.location.lng(), 6);
    }
  }

  // TODO: ShakeAlert integration code
}

void handleRoot() {
  server.send(200, "text/html", "<html><body><h1>ESP8266 Earthquake Warning System</h1><p>Click <a href=\"/wifi\">here</a> to change Wi-Fi password.</p></body></html>");
}

void handleWifi() {
  String html = "<html><body><h1>Change Wi-Fi Password</h1><form method=\"POST\" action=\"/update\"><label>SSID: <input type=\"text\" name=\"ssid\"></label><br><label>Password: <input type=\"password\" name=\"password\"></label><br><input type=\"submit\" value=\"Submit\"></form></body></html>";
  server.send(200, "text/html", html);
}

void handleUpdate() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  // TODO: update Wi-Fi credentials
}

void alert() {
  digitalWrite(FLASHLIGHT_PIN, HIGH);
  tone(SPEAKER_PIN, 1000, 1000);
  delay(1000);
  digitalWrite(FLASHLIGHT_PIN, LOW);
  noTone(SPEAKER_PIN);
}

