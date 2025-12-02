#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_timer.h>
#include <img_converters.h>
#include <Arduino.h>
#include "fb_gfx.h"
#include "camera_index.h"
#include "esp_http_server.h"

// Wi-Fi credentials
const char* ssid = "swatantra"; // Give your own WiFi's  Name 
const char* password = "11223344"; // Give your own Wifi's Address

// Telegram bot credentials
const char* botToken = "8075297653:AAF30NhhxIwlHLQP6U8thTOznux8XK6lDdQ"; // Create a bot in your telegram and give its id
const char* chatID = "6338095345"; //  give your telegram ID

WiFiClientSecure clientTCP;

// PIR & BUZZER
#define PIR_PIN 14
#define BUZZER_PIN 2

bool motionAllowed = true;                 
unsigned long lastTriggerTime = 0;
const unsigned long cooldownTime = 8000;

int lastPirState = LOW;  // Stores previous PIR state

// Camera model
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

void startCameraServer();

// ===================== SEND TELEGRAM PHOTO ======================
void sendPhotoTelegram(camera_fb_t * fb) {
  if (WiFi.status() != WL_CONNECTED) return;

  clientTCP.stop();
  clientTCP.setInsecure();

  if (!clientTCP.connect("149.154.167.220", 443)) {
    Serial.println("❌ Telegram connection failed");
    return;
  }

  String boundary = "ESP32CAMBOUNDARY";
  String startRequest = "--" + boundary + "\r\n";
  startRequest += "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n";
  startRequest += String(chatID) + "\r\n--" + boundary + "\r\n";
  startRequest += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
  startRequest += "⚠ Motion Detected!\r\n--" + boundary + "\r\n";
  startRequest += "Content-Disposition: form-data; name=\"photo\"; filename=\"image.jpg\"\r\n";
  startRequest += "Content-Type: image/jpeg\r\n\r\n";

  String endRequest = "\r\n--" + boundary + "--\r\n";
  int contentLength = startRequest.length() + fb->len + endRequest.length();

  String headers = "POST /bot" + String(botToken) + "/sendPhoto HTTP/1.1\r\n";
  headers += "Host: api.telegram.org\r\n";
  headers += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
  headers += "Content-Length: " + String(contentLength) + "\r\n\r\n";

  clientTCP.print(headers);
  clientTCP.print(startRequest);
  clientTCP.write(fb->buf, fb->len);
  clientTCP.print(endRequest);

  delay(50);
  clientTCP.stop();
  Serial.println("📸 Photo sent to Telegram");
}

// ===================== SETUP ===========================
void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 20;
  config.fb_count = 1;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("❌ Camera init failed!");
    return;
  }

  Serial.println("Camera Ready!");
  startCameraServer();
}

// ===================== MOTION LOGIC ===========================
void loop() {
  int pirState = digitalRead(PIR_PIN);

  // Cooldown active → ignore
  if (!motionAllowed) {
    if (millis() - lastTriggerTime >= cooldownTime) {
      motionAllowed = true;
      Serial.println("🟢 Motion detection re-enabled");
    }
    return;
  }

  // Detect motion only when PIR goes LOW → HIGH (state change)
  if (pirState == HIGH && lastPirState == LOW) {
    Serial.println("🚨 Motion Detected!");

    motionAllowed = false;                
    lastTriggerTime = millis();           

    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);

    camera_fb_t * fb = esp_camera_fb_get();
    if (fb) {
      sendPhotoTelegram(fb);
      esp_camera_fb_return(fb);
    }
  }

  // Save last PIR state
  lastPirState = pirState;
}
