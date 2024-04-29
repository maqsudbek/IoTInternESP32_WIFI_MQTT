
#include <Arduino.h>

// Include necessary libraries here

#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>


// Define global variables here

// WiFi credentials
const char *ssid = "IoTExperiment";
const char *password = "iotexperiment303ab";

// MQTT credentials
const char *mqtt_server = "mqtt.iotserver.uz";
const int mqtt_port = 1883;
const char *mqtt_user = "userTTPU";
const char *mqtt_password = "mqttpass";

// MQTT client unique ID
const char *mqtt_client_id = "ttpuIoTTEacherEsp32";

// MQTT topics
const char *globalLedTopic = "ttpu/iot/global/led";
const char *btnPressTopic = "ttpu/iot/teacher/btn";

// pinout
const int redPin = 32;
const int yellowPin = 33;
const int greenPin = 25;
const int buttonPin = 26;

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
// Hello world
/**********************************/
// LOOP //
void loop(void) 
{
  // Keep WiFi and MQTT connection alive
  // TODO: Keep WiFi and MQTT connection alive here

}
