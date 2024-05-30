# ESP32 WiFi with MQTT service

------------------------------------------------
## In this Arduino sketch, I've implemented a system that connects an ESP32 device to a Wi-Fi network and an MQTT broker. The device subscribes to a specific MQTT topic to receive LED control commands and publishes a message when a button is pressed. Here's a breakdown of what I've done:

## Include Libraries: I've included necessary libraries such as WiFi.h, PubSubClient.h, WebServer.h, and ArduinoJson.h to enable Wi-Fi connection, MQTT communication, web server functionality, and JSON parsing, respectively.
## Global Variables: I've defined global variables including Wi-Fi credentials, MQTT server details, MQTT topics, GPIO pin assignments, and an instance of the PubSubClient class.
## Setup Function: In the setup function, I've initialized serial communication, configured GPIO pins, connected to Wi-Fi, and connected to the MQTT broker.
## Loop Function: The loop function continuously checks for incoming MQTT messages and button press events.
## Hardware Setup: The setupHardware function initializes GPIO pins for LED output and button input.
## Wi-Fi Connection: The connectWiFi function attempts to connect to the specified Wi-Fi network using the provided credentials.
## MQTT Connection: The connectMQTT function establishes a connection to the MQTT broker using the provided server details and subscribes to a specific topic to receive LED control commands.
## MQTT Callback: The callback function is called whenever a message is received on the subscribed MQTT topic. It parses the JSON message and controls the LEDs accordingly.
## Publish Button Press: The publishButtonPress function publishes a message to a specific MQTT topic when the button is pressed.
## Handle Button Press: The handleButtonPress function checks if the button is pressed and calls the publishButtonPress function to publish a message accordingly.

## Overall, this sketch enables bidirectional communication between the ESP32 device and an MQTT broker, allowing remote control of LEDs and button press events over the MQTT protocol.

## Here's a Photo of the circuit I've made:!
![photo_2024-04-26_12-44-20 (2)](https://github.com/MukhlisaMirajiboyeva/IoTInternESP32_WIFI_MQTT/assets/167959077/93385573-bd03-4ff2-8c28-9bfaf0e6810b)
![photo_2024-04-26_12-44-20](https://github.com/MukhlisaMirajiboyeva/IoTInternESP32_WIFI_MQTT/assets/167959077/3a8c6c57-ab96-49f4-9e01-6e2868f1091e)
![photo_2024-04-26_12-48-57](https://github.com/MukhlisaMirajiboyeva/IoTInternESP32_WIFI_MQTT/assets/167959077/07d4d2c0-e293-4249-8522-fb9ae16d69ef)
