#include <AsyncTCP.h>
#include <AsyncMqttClient.h>

#include <Wire.h>
#include <WiFi.h>

/*using a function or global variable defined in a different file*/
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}

#define WIFI_SSID "xxxxxx"
#define WIFI_PASSWORD "xxxxxx"

// Raspberry Pi Mosquitto MQTT Broker
 #define MQTT_HOST IPAddress(192, 168, xx, xx)

// For a cloud MQTT broker, type the domain name
// #define MQTT_HOST "broker.mqttdashboard.com"

#define MQTT_PORT 1883

// MQTT Topics
// const char* MQTT_PUB_MOIS "ESP32/YL69/moisture";
#define MQTT_PUB_MOIS "ESP32/YL69/moisture"

// Sensor Pins
const int sensor_pin = 36;

// Variables to hold sensor readings
int sensorValue;
int moisturePercentage;

/*Create an AsyncMqttClient object called mqttClient to handle the MQTT client and timers to reconnect to your MQTT broker and router when it disconnects*/
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// auxiliary timer variables
unsigned long previousMillis = 0;   // Stores last time sensor value was published
const long interval = 10000;        // Interval at which to publish sensor readings


/*following functions come with the Async Mqtt Client library*/

// connect ESP32 to the router
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// connects ESP32 to the MQTT broker
void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

// handling the Wi-Fi events
void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {

    /*after a successful connection with the router and MQTT broker, it prints the ESP32 IP address*/
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;

    /*if the connection is lost, it starts a timer and tries to reconnect.*/
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

// after starting a session with the broker
void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

/*void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}*/

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

/*all these functions that above are callback functions. 
So, they are executed asynchronously.*/


void setup() {
  Serial.begin(115200);
  Serial.println();

  /*create timers that will allow both the MQTT broker and Wi-Fi connection to reconnect, in case the connection is lost.*/
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  /*assigns a callback function to execute the WiFiEvent()when the ESP32 connects to WiFi*/
  WiFi.onEvent(WiFiEvent);

  /*assign all the callbacks functions. This means that these functions will be executed automatically when needed.*/
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  // mqttClient.setCredentials("username", "password");
  connectToWifi();
}

void loop() {

  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  
  if (currentMillis - previousMillis >= interval) {
    
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    
    // New sensor readings
    sensorValue = analogRead(sensor_pin);
    // map the sensor reading to a percentage
    moisturePercentage = map(sensorValue,1300,4095,100,0);
    //Serial.println(moisturePercentage);
    
    // Publish an MQTT message on topic "ESP32/YL69/moisture"
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_MOIS, 1, true, String(moisturePercentage).c_str());   
    // uint16_t packetIdPub = mqttClient.publish(topic, QoS, retain flag, payload);
                               
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_MOIS, packetIdPub1);
    Serial.printf("Message: %i \n", moisturePercentage);

    /*
    use the publish() method on the mqttClient object to publish data on a topic. 
    The publish() method accepts the following arguments, in order:

      - MQTT topic (const char*)
      - QoS (uint8_t): quality of service – it can be 0, 1 or 2
            0: the message will be delivered once or not at all. The message is not acknowledged. There is no possibility of duplicated messages;
            1: the message will be delivered at least once, but may be delivered more than once;
            2: the message is always delivered exactly once;
      - retain flag (bool): retain flag
      - payload (const char*) – in this case, the payload corresponds to the sensor reading
    
    */
  }
}
