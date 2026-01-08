# Smart Plant Monitoring & Watering System 🌱

## Overview
This project presents a Smart Plant Monitoring and Watering System built using Arduino and RemoteXY. The system continuously monitors key environmental and plant parameters such as soil moisture, water level, temperature, humidity, light intensity, and motion to ensure optimal plant care.

Based on real-time sensor data, the system automatically controls plant watering through a relay-driven water pump and provides visual, audio, and mobile-based feedback. A 16×2 I2C LCD displays live readings and system messages, while alerts are generated using LEDs, a buzzer, a servo-based indicator, and voice notifications through a DFPlayer Mini module.

## Features
- Soil moisture monitoring
- Water level detection with servo indicator
- Temperature & humidity display (DHT11)
- Mobile monitoring via RemoteXY (WiFi)
- Automatic plant watering using relay-controlled pump
- Alerts using buzzer and LED
- LCD status display
- Voice alerts using DFPlayer Mini
- Day/Night detection using LDR
- Motion detection greeting system

## System Components
- Arduino Uno
- ESP8266 (RemoteXY WiFi mode)
- DHT11 Temperature & Humidity Sensor
- Soil Moisture Sensor
- Water Level Sensor
- Relay Module (Water Pump Control)
- Servo Motor
- LCD (I2C – 16x2)
- PIR Motion Sensor
- LDR (Light Sensor)
- Buzzer & LED
- DFPlayer Mini + Speaker

## Mobile Interface
The mobile control and monitoring interface was developed using RemoteXY, chosen for:
- Stable communication
- Faster prototyping
- Reduced Android-side complexity

The app displays:
- Soil moisture value
- Water level value

## How It Works
Sensors continuously monitor soil, water, light, temperature, and motion. If soil moisture is low, the system activates the water pump automatically. Water level is monitored and alerts are triggered when low. Environmental data is displayed on LCD and sent to the RemoteXY mobile app. Voice alerts play during day/night transitions.

## Libraries Used
- LiquidCrystal_I2C
- Servo
- DHT11
- SoftwareSerial
- DFRobotDFPlayerMini
- RemoteXY

## Conclusion
This project demonstrates a complete smart plant monitoring and watering system using Arduino and RemoteXY. It combines sensor data, automatic watering, LCD display, alerts, and mobile monitoring to ensure plants are cared for efficiently.
It’s designed for learning, experimentation, and practical use, and can be extended in the future with features like cloud logging, notifications, or multiple plant monitoring. This project is a strong example of embedded systems and IoT applied to smart agriculture.
