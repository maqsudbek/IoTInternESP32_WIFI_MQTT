
#include <Arduino.h>

// Include necessary libraries here
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Define global variables here
const char *ssid = "ab's Galaxy Note20 Ultra";
const char *password = "1tah0323";
const char *mqtt_broker_ip = "mqtt.iotserver.uz";
const int mqtt_port = 1883;
const char *mqtt_user = "userTTPU";
const char *mqtt_password = "mqttpass";
const char *led_topic = "ttpu/iot/global/led";
const char *btn_topic = "ttpu/iot/u13229/btn";
const int GREEN_PIN = 25;
const int YELLOW_PIN = 33;
const int RED_PIN = 32;
const int BUTTON_PIN = 26;

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
  Serial.printf("Message received,\nMessage: %s, Topic: %s\n", messageTemp, topic);
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

/**********************************/
// SETUP //
void setup(void) {
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

  //
}

/**********************************/
// LOOP //
void loop(void) {
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  } else {
    mqtt_client.loop();
  }
  if (digitalRead(BUTTON_PIN) == LOW) {
    mqtt_client.publish(btn_topic, "TEST");
  }
}
