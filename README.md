**Introduction**
-----------------

This project is a fully integrated Smart Home Monitoring and Burglar Alarm System, designed to provide complete, real-time protection for residential and small-scale environments. By combining two powerful systems—an environmental monitoring module and an intelligent security alarm—we created a unified platform that ensures safety, awareness, and automation at all times.
The Smart Home module continuously tracks temperature, humidity, smoke, harmful gases, air quality, atmospheric pressure, and altitude using sensors like DHT22, MQ-2, MQ-135, and BMP280. Fire detection is handled through a flame sensor, while a moving-average and hysteresis algorithm ensures stable and accurate gas-leak detection. All processed data is displayed on a Blynk IoT dashboard, providing users with live insights from anywhere in the world.
On the security side, the Burglar Alarm module uses ESP32-CAM to instantly capture images when motion or intrusion is detected. These captured photos are then sent directly to the user via Telegram Bot API, delivering fast, reliable notifications even when the user is away from home.
By merging these two systems, the project delivers an all-in-one solution that offers environmental monitoring, intrusion detection, automated alerts, and remote surveillance. This integration makes the system highly practical, scalable, and ideal for modern smart-home applications.



# Smart Home Monitoring System
-------------------------------

## 🚀 Features
- Live temperature & humidity monitoring (DHT22)
- Gas & smoke detection (MQ‑2 with smoothing algorithm)
- Air quality detection (MQ‑135)
- Fire detection (Flame Sensor)
- Pressure & altitude measurement (BMP280)
- Real-time UI on Blynk dashboard
- Moving‑average smoothing for stable gas detection
- Hysteresis logic to avoid false alerts

## 🛠 Hardware Required
- ESP32 Development Board
- DHT22 Sensor
- MQ‑2 Gas/Smoke Sensor
- MQ‑135 Air Quality Sensor
- Flame Sensor
- BMP280 Pressure Sensor
- Jumper wires
- Breadboard
- USB cable

## 📌 Pin Connections

| Sensor | ESP32 Pin |
|--------|-----------|
| Flame Sensor (DO) | GPIO 4 |
| DHT22 Data | GPIO 18 |
| MQ‑135 (AO) | GPIO 34 |
| MQ‑2 (AO) | GPIO 33 |
| BMP280 (I2C) | SDA → 21, SCL → 22 |

---

## 📡 Blynk Setup

1. Install **Blynk IoT app**
2. Create a new **Template**
3. Copy:
   - Template ID  
   - Template Name  
   - Auth Token  
4. Add datastreams:
   - V0 → Flame
   - V1 → Pressure
   - V2 → Altitude
   - V3 → MQ‑135
   - V4 → Temperature
   - V5 → Humidity
   - V6 → MQ‑2 smooth value

Update the code with your values.

---

## 🔌 Install Required Libraries

Install these from Arduino Library Manager:

- **Blynk**
- **DHT Sensor Library**
- **Adafruit BMP280**
- **Adafruit Unified Sensor**

Board Manager:

ESP32 by Espressif Systems

Install it.

---

## 📶 WiFi Setup

Edit these lines:


char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

---

## 🧠 Working Principle
- DHT22 reads temperature & humidity
- Flame sensor detects fire (LOW = fire)
- MQ‑135 detects CO₂ level (raw ADC values)
- MQ‑2 uses:
  - Moving average filter
  - Dynamic threshold
  - Hysteresis alert logic
- BMP280 gives pressure/altitude
- All values are sent to Blynk every 2 seconds

## 🧪 Output on Serial Monitor

Example output:

Temp: 28.4 °C | Humidity: 55.1 %
Fire: No Fire
MQ135 Raw: 880
Pressure: 1011.4 hPa | Altitude: 18.2 m
MQ2 Smoothed: 1050


## 🖼 Dashboard UI (Blynk)

Suggested widgets:
- Gauges (Temp, Humidity)
- LEDs (Flame alert)
- Charts (MQ‑2, MQ‑135)
- Labels (Pressure, Altitude)

---

## 📂 Project Structure


SmartHomeMonitoringSystem/
 ├── README.md
 ├── src/
 │    └── SmartHomeMonitoring.ino
 ├── images/
 │    └── wiring_diagram.png
 └── docs/
      └── system_overview.docx


---

## ✔ Future Improvements
- Add Telegram alerts
- Add relay control for automation
- Add battery power & solar charging
- Add AI prediction for gas leakage


ESP32-CAM Burglar Alarm System with Telegram Alerts
==================================================

A DIY home-security system built using ESP32-CAM that captures images whenever motion is detected and instantly sends them to your Telegram bot. You receive real-time intrusion alerts directly on your phone — fast, reliable, and easy to deploy.

FEATURES
--------
- Motion detection (PIR or software)
- Captures high-resolution images
- Sends photos instantly to Telegram bot
- WiFi-enabled alerts
- Auto-reconnect logic
- Optional LED Flash control

HARDWARE REQUIREMENTS
---------------------
- ESP32-CAM (AI Thinker)
- FTDI Programmer (3.3V)
- PIR Motion Sensor (HC-SR501)
- Jumper Wires
- External 5V power supply

WIRING
------
FTDI → ESP32-CAM  
5V → 5V  
GND → GND  
U0R → U0T  
U0T → U0R  
GPIO0 → GND (for flashing)

PIR → ESP32-CAM  
VCC → 5V  
GND → GND  
OUT → GPIO13

SOFTWARE REQUIREMENTS
---------------------
- Arduino IDE
- ESP32 board package
- Default ESP32 libraries

INSTALLING ESP32 BOARD PACKAGE
------------------------------
Arduino → File → Preferences → Additional URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

LIBRARIES USED
--------------
- esp_camera.h
- WiFi.h
- WiFiClientSecure.h
- esp_timer.h
- img_converters.h
- fb_gfx.h
- esp_http_server.h

TELEGRAM BOT SETUP
------------------
1. Open Telegram  
2. Search: @BotFather  
3. /newbot  
4. Get bot token  
5. Get Chat ID via:  
https://api.telegram.org/bot<token>/getUpdates

FLASHING ESP32-CAM
------------------
1. Connect FTDI  
2. GPIO0 → GND  
3. Select Board: AI Thinker ESP32-CAM  
4. Upload  
5. Remove GPIO0  
6. Reset

PROJECT STRUCTURE
-----------------
ESP32-CAM-Burglar-Alarm/  
 ├── src/main.ino  
 ├── images/  
 ├── docs/  
 └── README.md

TROUBLESHOOTING
---------------
- Use 2.4 GHz WiFi  
- Check loose camera ribbon  
- Provide 5V 2A power  
- Verify Telegram token & chat ID  
- Try sending a text message first  

FUTURE IMPROVEMENTS
-------------------
- SD card storage  
- Buzzer alarm  
- Web dashboard  
- Face detection  
- DeepSleep mode  
- MQTT/Firebase alerts


-------
MIT License

