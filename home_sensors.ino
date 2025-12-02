/***************************************************
 * BLYNK TEMPLATE CONFIGURATION
 ***************************************************/
#define BLYNK_TEMPLATE_ID "TMPL329QbVPs2"
#define BLYNK_TEMPLATE_NAME "Smart Home Monitoring System"
#define BLYNK_AUTH_TOKEN "Oc8oV18IZ3U0xLCg0XmSYkQicjuD4JwG"

/***************************************************
 * Library Includes
 ***************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHTesp.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

/***************************************************
 * WiFi Credentials
 ***************************************************/
char ssid[] = "swatantra";
char pass[] = "11223344";

/***************************************************
 * Pin Definitions
 ***************************************************/
#define FLAME_DO 4
#define DHT_PIN 18
#define MQ135_PIN 34
#define MQ2_PIN 33   

/***************************************************
 * MQ-2 Moving Average Variables
 ***************************************************/
const int NUM_SAMPLES = 10;
int samples[NUM_SAMPLES];
int sampleIndex = 0;
long sampleSum = 0;

const int BASELINE = 1200;
const int THRESHOLD = BASELINE + 500;
const int HYSTERESIS = 200;

bool inDanger = false;

/***************************************************
 * Sensor Objects
 ***************************************************/
DHTesp dht;
Adafruit_BMP280 bmp;
BlynkTimer timer;

/***************************************************
 * MQ2: Moving Average Read Function
 ***************************************************/
int getSmoothedMQ2() {
  int newVal = analogRead(MQ2_PIN);
  sampleSum -= samples[sampleIndex];
  samples[sampleIndex] = newVal;
  sampleSum += newVal;
  sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;
  return sampleSum / NUM_SAMPLES;
}

/***************************************************
 * Function: Send Sensor Data
 ***************************************************/
void sendSensorData() {

  // --- DHT22 ---
  TempAndHumidity dhtData = dht.getTempAndHumidity();
  if (!isnan(dhtData.temperature)) {
    Blynk.virtualWrite(V4, dhtData.temperature);
    Blynk.virtualWrite(V5, dhtData.humidity);

    Serial.printf("ðŸŒ¡ Temp: %.2fÂ°C | ðŸ’§ Humidity: %.2f%%\n",
                  dhtData.temperature, dhtData.humidity);
  }

  // --- Flame Sensor ---
  int flame = digitalRead(FLAME_DO);
  Blynk.virtualWrite(V0, flame == LOW);

  Serial.println(flame == LOW ? "ðŸ”¥ Fire Detected!" : "âœ” No Fire");

  // --- MQ-135 ---
  int mq135_value = analogRead(MQ135_PIN);
  Serial.printf("ðŸŒ« MQ135 Raw: %d\n", mq135_value);
  Blynk.virtualWrite(V3, mq135_value);

  // --- BMP280 ---
  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(1013.25);

  Blynk.virtualWrite(V1, pressure);
  Blynk.virtualWrite(V2, altitude);

  Serial.printf("â›° Pressure: %.2f hPa | Alt: %.2f m\n", pressure, altitude);

  // ****** MQ-2 LOGIC (Updated) ******
  int smooth = getSmoothedMQ2();
  Serial.printf(" MQ2 Smoothed: %d\n", smooth);

  // SEND VALUE TO BLYNK (V6) âœ”
  Blynk.virtualWrite(V6, smooth);

  if (!inDanger) {
    if (smooth > THRESHOLD) {
      inDanger = true;
      Serial.println(" MQ-2 ALERT: Gas/Smoke Detected!");
    }
  } else {
    if (smooth < (THRESHOLD - HYSTERESIS)) {
      inDanger = false;
      Serial.println(" MQ-2 SAFE: Cleared.");
    }
  }

  Serial.println("--------------------------------");
}

/***************************************************
 * Setup
 ***************************************************/
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(FLAME_DO, INPUT);
  dht.setup(DHT_PIN, DHTesp::DHT22);

  // Init BMP280
  if (!bmp.begin(0x76)) Serial.println("âŒ BMP280 NOT FOUND!");
  else Serial.println(" BMP280 OK");

  //  Initialize MQ-2 buffer
  for (int i = 0; i < NUM_SAMPLES; i++) {
    int v = analogRead(MQ2_PIN);
    samples[i] = v;
    sampleSum += v;
    delay(50);
  }

  Serial.println("MQ-2 Calibration Complete");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendSensorData);

  Serial.println(" SmartHome Monitoring Started");
}

/***************************************************
 * Loop
 ***************************************************/
void loop() {
  Blynk.run();
  timer.run();
}
