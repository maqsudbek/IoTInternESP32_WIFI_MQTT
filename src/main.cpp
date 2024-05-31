#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char* ssid = "IoTPrivate";
const char* password = "iotprivate303";
WebServer server(80); // Create a web server on port 80

// MQTT credentials
const char* mqtt_server = "tcp://192.168.10.105";// IP of MQTT broker
const int mqtt_port = 1883; // MQTT port
const char* mqtt_username = "spirealm";
const char* mqtt_password = "spirealm";

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
  client.loop(); // Handle MQTT tasks
  handleButtonPress(); // Check for button press
 } 

void setupHardware() {
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);// Setup button pin with internal pull-up resistor
}

// Connect to the external WiFi network
void connectWiFi() {
  WiFi.disconnect(true);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password); 
  
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
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_subscribe); // Subscribe to topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000); // Wait 2 seconds before retrying
    }
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
