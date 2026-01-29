#include <WiFi.h>
#include <HTTPClient.h>

/* ===================== USER CONFIG ===================== */

const char* WIFI_SSID = "SSID";
const char* WIFI_PASS = "PASSW";

const char* SLACK_WEBHOOK = API_TOKEN;

#define SENSOR_PIN 34
#define VIB_THRESHOLD 300
#define REQUIRED_TIME 10000UL
#define SAMPLE_DELAY 50

/* ===================== GLOBALS ===================== */

unsigned long vibrationStart = 0;
bool vibrationActive = false;
bool onMessageSent = false;
bool alert20sSent = false;

/* ===================== WIFI FUNCTION ===================== */

void connectWiFi() {
  Serial.print("Connecting to WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 20000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi FAILED");
  }
}

/* ===================== SLACK ===================== */

void sendSlack(String msg) {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    if (WiFi.status() != WL_CONNECTED) return;
  }

  HTTPClient http;
  http.begin(SLACK_WEBHOOK);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"text\":\"" + msg + "\"}";
  int httpCode = http.POST(payload);

  Serial.print("Slack HTTP code: ");
  Serial.println(httpCode);

  http.end();
}

/* ===================== SETUP ===================== */

void setup() {
  Serial.begin(115200);
  delay(1000);

  analogReadResolution(12);

  connectWiFi();

  sendSlack("ESP32 online. Generator monitoring started.");
}

/* ===================== LOOP ===================== */

void loop() {

  int vibration = analogRead(SENSOR_PIN);
  Serial.print("Vibration: ");
  Serial.println(vibration);

  if (vibration > VIB_THRESHOLD) {

    if (!vibrationActive) {
      vibrationActive = true;
      vibrationStart = millis();
      onMessageSent = false;
      alert20sSent = false;
      Serial.println("Vibration started...");
    }

    if (!onMessageSent) {
      sendSlack("Generator vibration detected. Equipment is ON.");
      onMessageSent = true;
    }

    if (!alert20sSent && millis() - vibrationStart >= REQUIRED_TIME) {
      sendSlack("Generator vibration HIGH for more than 10 seconds.");
      alert20sSent = true;
    }

  } else {
    vibrationActive = false;
    vibrationStart = 0;
  }

  delay(SAMPLE_DELAY);
}
