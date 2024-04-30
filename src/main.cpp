//============================= Include necessary libraries here ================================
// TODO: Add necessary libraries here
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

//============================= Define global variables here =============================
// TODO: Add global variables here
#define red 32
#define yellow 33
#define green 25

#define buttonPin 26

const char* ssid = "IoTPrivate";
const char* password = "iotprivate303";
unsigned long ttime;
unsigned long ttimeB;


const char *mqtt_broker = "mqtt.iotserver.uz";
const char *subTopic = "ttpu/iot/global/led";
const char *mqtt_username = "userTTPU";
const char *mqtt_password = "mqttpass";
const int mqtt_port = 1883;
const char *topic = "ttpu/iot/UsmonovQ/btn";


String buffer;


LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient espClient;
PubSubClient client(espClient);


//============================= Define functions here =============================
// TODO: Add functions here
void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(subTopic);
 Serial.print("Message:");
 buffer = "";
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
     buffer+=(char)payload[i];
 }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(buffer);
  if(buffer.indexOf("red") != -1){
    if(buffer.indexOf("on") != -1){
      digitalWrite(red, HIGH);
    }else if(buffer.indexOf("off") != -1){
      digitalWrite(red, LOW);
    }
  }else if(buffer.indexOf("yellow") != -1){
    if(buffer.indexOf("on") != -1){
      digitalWrite(yellow, HIGH);
    }else if(buffer.indexOf("off") != -1){
      digitalWrite(yellow, LOW);
    }
  }else if(buffer.indexOf("green") != -1){
    if(buffer.indexOf("on") != -1){
      digitalWrite(green, HIGH);
    }else if(buffer.indexOf("off") != -1){
      digitalWrite(green, LOW);
    }
  }
 Serial.println();
 Serial.println("-----------------------");
}

/**********************************/
// SETUP //
void setup(void) 
{
  //============================= Initialize peripherals =============================
  // TODO: Initialize peripherals here
  // TODO: LEDS, Buttons, LCD, etc.
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  //============================= Connect to WiFi =============================
  // TODO: Connect to WiFi hereSerial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //============================= Setup MQTT =============================
  // TODO: Setup MQTT here
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "nursulton03";
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 client.subscribe(subTopic);
  // 

}
/**********************************/
// LOOP //
void loop(void) 
{
  //============================= Keep WiFi and MQTT connection alive =============================
  // TODO: Keep WiFi and MQTT connection alive here
  client.loop();
  if ((WiFi.status() != WL_CONNECTED) && (millis() - ttime >=10000)) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    ttime = millis();
  }

  if(!digitalRead(buttonPin) && millis() - ttimeB >= 1000){
    ttimeB = millis();
    client.publish(topic, "pressed");
    Serial.println("pressed");
  }
}