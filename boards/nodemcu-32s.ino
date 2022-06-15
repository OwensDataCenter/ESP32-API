#include <WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include "SimpleBLE.h"
#include "time.h"
#include "soc/rtc.h"
const char* ssid = "SSID"; //Change this to your WiFi SSID
const char* password = "PASS"; //Change this to your WiFi Password
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  15
#define BUTTON_PIN_BITMASK 0x200000000
HTTPClient http;
SimpleBLE ble;
RTC_DATA_ATTR int bootCount = 0;
void setup() {
  ++bootCount;
  if (bootCount == 128) {
    ESP.restart();
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    WiFi.disconnect(true);
    WiFi.begin(ssid, password);
  }
  ble.begin("ESP32-BLE");
  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("ERROR: TIME NOT SET");
  }
  Serial.println(&timeinfo, "%B %d %Y %H:%M:%S");
  int rssi = WiFi.RSSI();
  http.begin("http://opservernetwork.ddns.net:81/?battery=0&devid=dev10c7979d9309dc9ac1f478198f73d3e8&rssi=" + String(rssi) + "&bootcount=" + String(bootCount));
  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String syspayload = http.getString();
      Serial.println(syspayload);
    }
  }
  http.end();
  http.begin("http://speedtest.ohiordc.rr.com:8080/download?size=1000");
  int httpcode = http.GET();
  if (httpcode > 0) {
    if (httpcode == HTTP_CODE_OK) {
      String payload = http.getString();
    }
  }
  http.end();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  digitalWrite(2, LOW);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  esp_deep_sleep_start();
}
void loop() {
}
