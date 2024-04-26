#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
//#include <LiquidCrystal_I2C.h>

const char* ssid = "IoTExperiment";
const char* password = "iotexperiment303ab";
WebServer server(80);

const char* mqtt_server = "mqtt.iotserver.uz";
const int mqtt_port = 1883;
const char* mqtt_username = "userTTPU";
const char* mqtt_password = "mqttpass";

const char* mqtt_topic_subscribe = "ttpu/iot/global/led";
const char* mqtt_topic_publish = "ttpu/iot/mukhlisa/btn";


const int redLedPin = 32;
const int yellowLedPin = 33;
const int greenLedPin = 25;
const int buttonPin = 26;

WiFiClient espClient;
PubSubClient client(espClient);

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
  handleButtonPress(); }

void setupHardware() {
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  Serial.println(message);

  DynamicJsonDocument doc(128);
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

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



void publishButtonPress() {
  if (client.connected()) {
    client.publish(mqtt_topic_publish, "pressed");
    Serial.println("Button press event published");
  } else {
    Serial.println("MQTT client not connected");
  }
}

void handleButtonPress() {

  if (digitalRead(buttonPin) == LOW) {
    publishButtonPress();
    delay(1000); 
  }
}
