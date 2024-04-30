#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
const char* ssid = "IoTLaB";
const char* password = "iot303abab";

// MQTT Broker settings
const char* mqtt_broker = "192.168.10.112";
const char* mqtt_username = "Mirmir";
const char* mqtt_password = "";
const int mqtt_port = 1883;

// MQTT Topics
const char* mqtt_publish_topic = "ttpu/iot/Mirmir/btn"; // Replace 'username' with your unique username
const char* mqtt_subscribe_topic = "ttpu/iot/global/led";

// GPIO assignments
const int buttonPin = 26;
const int redLED = 32;
const int yellowLED = 33;
const int greenLED = 25;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient espClient;
PubSubClient client(espClient);

void setupWiFi() {
  lcd.clear();
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  lcd.setCursor(0, 1);
  lcd.print("WiFi Connected!");
}

void reconnect() {
  lcd.clear();
  lcd.print("Connecting MQTT");
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) { // Ensure unique client ID
      Serial.println("Connected to MQTT Broker!");
      lcd.setCursor(0, 1);
      lcd.print("MQTT Connected!");
      client.subscribe(mqtt_subscribe_topic);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  lcd.clear();
  lcd.print("MQTT Msg Recvd");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  // Example JSON parsing
  if (payload[0] == '{') {
    // Simple JSON parsing
    if (strstr((char*)payload, "\"red\":\"on\"")) digitalWrite(redLED, HIGH);
    if (strstr((char*)payload, "\"red\":\"off\"")) digitalWrite(redLED, LOW);
    if (strstr((char*)payload, "\"yellow\":\"on\"")) digitalWrite(yellowLED, HIGH);
    if (strstr((char*)payload, "\"yellow\":\"off\"")) digitalWrite(yellowLED, LOW);
    if (strstr((char*)payload, "\"green\":\"on\"")) digitalWrite(greenLED, HIGH);
    if (strstr((char*)payload, "\"green\":\"off\"")) digitalWrite(greenLED, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  setupWiFi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    client.publish(mqtt_publish_topic, "pressed");
    lcd.clear();
    lcd.print("Button Pressed!");
  }
  lastButtonState = currentButtonState;
  delay(100); // Debounce delay

  // Optionally, you can update the display periodically or on certain events
  // This is just a placeholder for any additional display updates you might want
  // For example, you could display dynamic data or system status
}