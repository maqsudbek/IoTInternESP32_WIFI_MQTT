// INCLUDE NECESSARY LIBRARIES HERE
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// DEFINE GLOBAL VARIABLES HERE

#define lcdAddress 0x27 // link to the address number of LCD
#define LED_R 32
#define LED_G 25
#define LED_Y 33
#define BUTTON 26
#define SDA 21
#define SCL 22

const char* ssid = "IoTPrivate";
const char* password = "iotprivate303";
const char* mqtt_server = "192.168.10.111";
const int mqtt_port = 1883;
const char* mqtt_username = "uNsPeCiFiEd";
const char* mqtt_password = "HelloUzzer";
const char* myID = "OtabekDavronbek11";
const char* pubTopic = "ttpu/iot/OtabekDavronbek/btn";
const char* subTopic = "ttpu/iot/global/led";

unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 10000;

LiquidCrystal_I2C lcd(lcdAddress, 16, 2); // initializing LCD with the address, columns, rows
WiFiClient espClient; 
PubSubClient client(espClient);

// DEFINE FUNCTIONS HERE 

void callbackmqtt(char* topic, byte* payload, unsigned int length);

    // Function to set up WiFi connection
void setup_wifi() {
  delay(10);
  Serial.println();
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
}

    //Function to set up MQTT
void setup_mqtt(){
  client.subscribe(subTopic);
}

    //Function to reconnect to MQTT
void reconnect(){
  if(client.connected()){
    return;
  }

  if(millis() - lastReconnectAttempt > reconnectInterval) {
    lastReconnectAttempt = millis();
    Serial.print("Attempting MQTT connection...");
    if(client.connect(myID, mqtt_username, mqtt_password)){
      Serial.println("connected");
      setup_mqtt();
    }
    else{
      Serial.print("failure, rc = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}


void callbackmqtt(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Convert payload to a string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);

  // Toggle LEDs and display message on LCD
  if (message.indexOf("red") != -1) {
    if(message.indexOf("on") != -1) 
        {digitalWrite(LED_R, 1);}
    else{digitalWrite(LED_R, 0);}
  }
  if (message.indexOf("green") != -1) {
    if(message.indexOf("on") != -1)
        {digitalWrite(LED_G, 1);}
    else{digitalWrite(LED_G, 0);}
  }
  if (message.indexOf("yellow") != -1) {
    if(message.indexOf("on") != -1)
        {digitalWrite(LED_Y, 1);}
    else{digitalWrite(LED_Y, 0);}
  }
}

/**********************************/
// SETUP //
void setup(void) 
{
  Serial.begin(115200);

  // Initialize peripherals
  Wire.begin(SDA, SCL); // initialize I2C pins
  lcd.begin(16, 2); // connect to LCD with 16 cols and 2 rows
  lcd.backlight(); // turn backlight on
  lcd.setCursor(3, 0); 
  lcd.print("WAY AHEAD!");

  pinMode(BUTTON, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);

  // Connect to WiFi
  setup_wifi();
  
  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackmqtt);

  // Attempting connection
  reconnect();
}

/**********************************/
// LOOP //
void loop(void) 
{
  // Keep WiFi and MQTT connection alive
  if(WiFi.status() != WL_CONNECTED){
    setup_wifi();
  }
  if(!client.connected()){
    reconnect();
  }

  client.loop(); 

  //Publish message when the button is pressed
  if(digitalRead(BUTTON) == 0){
    client.publish(pubTopic, "pressed");
    Serial.println("BUTTON PRESSED");
  }
}