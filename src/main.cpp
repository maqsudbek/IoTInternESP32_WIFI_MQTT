
#include <Arduino.h>
#include <PubSubClient.h> // Для MQTT
#include <WiFi.h>


// Include necessary libraries here
// TODO: Add necessary libraries here

// Define global variables here
// TODO: Add global variables here

// Define functions here
// TODO: Add functions here

/**********************************/
// SETUP //
void setup(void) 
{
  // Initialize peripherals
  // TODO: Initialize peripherals here
  // TODO: LEDS, Buttons, LCD, etc.

  // Connect to WiFi
  // TODO: Connect to WiFi here

  // Setup MQTT
  // TODO: Setup MQTT here

  // 

}

const char* ssid = "YOUR_SSID"; // Имя WiFi сети
const char* password = "YOUR_PASSWORD"; // Пароль от WiFi
const char* mqtt_server = "broker_address"; // Адрес MQTT брокера

WiFiClient espClient;
PubSubClient client(espClient);



/**********************************/
// LOOP //
void loop(void) 
{
  // Keep WiFi and MQTT connection alive
  // TODO: Keep WiFi and MQTT connection alive here
}
