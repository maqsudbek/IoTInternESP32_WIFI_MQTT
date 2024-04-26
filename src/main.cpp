#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

// WiFi settings
const char* ssid = "IoTExperiment";
const char* password = "iotexperiment303ab";


// MQTT settings
const char* mqtt_server = "mqtt.iotserver.uz";
const int mqtt_port = 1883;
const char* mqtt_user = "userTTPU";
const char* mqtt_password = "mqttpass";

// GPIO pins
const int RED_LED_PIN = 32;
const int YELLOW_LED_PIN = 33;
const int GREEN_LED_PIN = 25;
const int BUTTON_PIN = 26;

//MQTT topics
const char *globalTopic="ttpu/iot/global/btn";
const char *btnPressTopic="ttpu/iot/mironshoh/btn";

//MQTT client unique id
const char *mqtt_client_id="ttpuIoTSU13231";


WiFiClient espClient;
PubSubClient client(espClient);

String getClientId() {
  String macAddress = WiFi.macAddress();
  macAddress.replace(":", "");
  return "ESP32_" + macAddress;
}

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String msg;
  for(int i = 0; i < length; i++){
    msg = msg + (char)payload[i];
  }
  Serial.println(msg);

  DynamicJsonDocument doc(128);
  DeserializationError error = deserializeJson(doc, payload, length);
  
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  if (strncmp(topic, "ttpu", 4) == 0) {
    if (doc.containsKey("red")) {
      if (strcmp(doc["red"], "on") == 0) {
        digitalWrite(RED_LED_PIN, HIGH);
      } else if (strcmp(doc["red"], "off") == 0) {
        digitalWrite(RED_LED_PIN, LOW);
      }
    }
    if (doc.containsKey("yellow")) {
      if (strcmp(doc["yellow"], "on") == 0) {
        digitalWrite(YELLOW_LED_PIN, HIGH);
      } else if (strcmp(doc["yellow"], "off") == 0) {
        digitalWrite(YELLOW_LED_PIN, LOW);
      }
    }
    if (doc.containsKey("green")) {
      if (strcmp(doc["green"], "on") == 0) {
        digitalWrite(GREEN_LED_PIN, HIGH);
      } else if (strcmp(doc["green"], "off") == 0) {
        digitalWrite(GREEN_LED_PIN, LOW);
      }
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(getClientId().c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("ttpu/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (digitalRead(BUTTON_PIN) == LOW) {
    client.publish("ttpu/iot/mironshoh/btn", "pressed");
    delay(1000); // debounce delay
  }
}