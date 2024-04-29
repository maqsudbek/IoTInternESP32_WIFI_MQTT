
#include <Arduino.h>

// Include necessary libraries here
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Define global variables here
// WiFi credentials
const char *ssid = "ab's Galaxy Note20 Ultra";
const char *password = "1tah0323";
// MQTT credentials
const char *mqtt_broker_ip = "192.168.237.157";
const int mqtt_port = 1883;
const char *mqtt_user = "userTTPU";
const char *mqtt_password = "mqttpass";
// MQTT topics
const char *led_topic = "ttpu/iot/global/led";
const char *btn_topic = "ttpu/iot/u13229/btn";
// Pins
const int GREEN_PIN = 25;
const int YELLOW_PIN = 33;
const int RED_PIN = 32;
const int BUTTON_PIN = 26;

// Button debouncing
unsigned long debounce_millis = -1000;
const unsigned long debounce_threshold = 200;

WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define functions here
void mqtt_reconnect() {
  while (not mqtt_client.connected()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Attempting MQTT connection...\n");
    Serial.print("Attempting MQTT connection...\n");
    if (mqtt_client.connect("u13229_client", mqtt_user, mqtt_password)) {
      mqtt_client.subscribe(led_topic);
    }
    Serial.print("Connected!\n");
  }
}

void mqtt_callback(char *topic, byte *message, int length) {
  String messageTemp = "";
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Message received,\nMessage: %s, Topic: %s\n", messageTemp, topic);
  Serial.printf("Message received,\nMessage: %s, Topic: %s\n", messageTemp,
                topic);
  int result_pin = -1;
  if (messageTemp.indexOf("}") > messageTemp.indexOf("{") &&
      messageTemp.indexOf("{") != -1 && messageTemp.indexOf("}") != -1) {
    if (messageTemp.indexOf("red") != -1) {
      result_pin = RED_PIN;
    } else if (messageTemp.indexOf("green") != -1) {
      result_pin = GREEN_PIN;
    } else if (messageTemp.indexOf("yellow") != -1) {
      result_pin = YELLOW_PIN;
    };
    if (messageTemp.indexOf("on") != -1 && result_pin != -1) {
      digitalWrite(result_pin, HIGH);
    } else if (messageTemp.indexOf("off") != -1 && result_pin != -1) {
      digitalWrite(result_pin, LOW);
    }
  }
}

void check_button() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (millis() - debounce_millis < debounce_threshold) {
      debounce_millis = millis();
      return;
    }
    debounce_millis = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("Sent message to topic:\n%s\n", btn_topic);
    Serial.printf("Sent message to topic:\n%s\n", btn_topic);
    mqtt_client.publish(btn_topic, "TEST");
  }
}

/**********************************/
// SETUP //
void setup(void) {
  // Initialize peripherals
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(26, INPUT_PULLUP);

  delay(100);

  // Connect to WiFi
  Serial.printf("Connecting to WiFI. ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFI. ");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(". ");
    // lcd.printf(". ");
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("\nConnected, ip is %s", WiFi.localIP().toString());
  Serial.printf("\nConnected, ip is %s", WiFi.localIP().toString());

  // Setup MQTT
  mqtt_client.setServer(mqtt_broker_ip, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);

}

/**********************************/
// LOOP //
void loop(void) {
  // Keep WiFi and MQTT connection alive
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  } else {
    mqtt_client.loop();
  }
  check_button();
}