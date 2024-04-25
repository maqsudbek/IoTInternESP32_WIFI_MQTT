# ESP32 WiFi with MQTT service

------------------------------------------------
## Description

- ESP32 is connected to WiFi AP
- Connect also to MQTT broker
- It subsribes to "ttpu/iot/global" topic
- It publishes msg to "ttpu/iot/username/msg" topic
- Whenever it receives something, it should print it to the LCD