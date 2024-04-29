#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Check if the JSON object has "red", "yellow", and "green" keys
const char* ssid = "IoTExperiment";
const char* password = "iotexperiment303ab";
WebServer server(80);

// MQTT credentials
const char* mqtt_server = "mqtt.iotserver.uz";
const int mqtt_port = 1883;
const char* mqtt_username = "userTTPU";
const char* mqtt_password = "mqttpass";

// MQTT client unique ID
const char *mqtt_client_id = "ttpuMukhlisa";

// MQTT topics
const char* mqtt_topic_subscribe = "ttpu/iot/global/led";
const char* mqtt_topic_publish = "ttpu/iot/mukhlisa/btn";

// GPIO pin assignments
const int redLedPin = 32;
const int yellowLedPin = 33;
const int greenLedPin = 25;
const int buttonPin = 26;

WiFiClient espClient;
PubSubClient client(espClient);

// Initialize peripherals
void setupHardware();
void connectWiFi();
void connectMQTT();
void callback(char* topic, byte* payload, unsigned int length);
void publishButtonPress();
void handleButtonPress();

void setup() {
  Serial.begin(115200);
  setupHardware();
  connectWiFi();
  connectMQTT();
}

void loop() {
  client.loop();
  handleButtonPress(); //Check for button press
 } 

void setupHardware() {
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void connectWiFi() {
  WiFi.disconnect(true);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password); // Connect to the external WiFi network
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Connect to the MQTT broker
void connectMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  if (client.connect(mqtt_username, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT broker");
    client.subscribe(mqtt_topic_subscribe);
  } else {
    Serial.print("MQTT connection failed, rc=");
    Serial.println(client.state());
  }
}

// Handle MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // Convert the byte array payload to a string
  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0'; // Null-terminate the string
  Serial.println(message); // Print the received message

// Parse the message to control LEDs
  DynamicJsonDocument doc(128); // JSON document to parse the message
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }
 // Check if the JSON object has "red", "yellow", and "green" keys
  if (doc.containsKey("red")) {
    const char* redState = doc["red"];
    digitalWrite(redLedPin, strcmp(redState, "on") == 0 ? HIGH : LOW);
    Serial.println("Red LED controlled successfully");
  } 

  if (doc.containsKey("yellow")) {
    const char* yellowState = doc["yellow"];
    digitalWrite(yellowLedPin, strcmp(yellowState, "on") == 0 ? HIGH : LOW);
    Serial.println("Yellow LED controlled successfully");
  }

  if (doc.containsKey("green")) {
    const char* greenState = doc["green"];
    digitalWrite(greenLedPin, strcmp(greenState, "on") == 0 ? HIGH : LOW);
    Serial.println("Green LED controlled successfully");
  }
  else {
    Serial.println("No LED control command found in the message");
  }
}


// Publish button press event to MQTT
void publishButtonPress() {
  if (client.connected()) {
    client.publish(mqtt_topic_publish, "pressed");
    Serial.println("Button press event published");
  } else {
    Serial.println("MQTT client not connected");
  }
}

// Handle button press events
void handleButtonPress() {
  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW) {
     // Publish button press event
    publishButtonPress();
    delay(1000); // Debounce delay
  }
}
