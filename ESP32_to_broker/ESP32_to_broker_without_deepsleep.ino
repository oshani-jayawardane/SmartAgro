/* Smart Agro - send data from ESP32 to MQTT broker - final draft
   PubSubClinet library - https://pubsubclient.knolleary.net/
   PubSubClient API Documentation: https://pubsubclient.knolleary.net/api.html#disconnect
   Debbuging help: PubSubClient is not fully compatible with the ESP32
*/

#include <PubSubClient.h>           /*Connect and publish to the MQTT broker*/
#include <WiFi.h>                   /*Enables the ESP32 to connect to the local network (via WiFi)*/

#define sensor_pin 36               /*sensor Pin */

int publishDelay = 5 * 1000;       /* Delay time after publishing sensor data (5 sec)*/
int setupDelay = 2000;              /* Delay time before connecting to Wi-Fi (2 sec) */

/*WiFi*/
const char* ssid = "Dialog 4G";              /* personal network SSID */
const char* wifi_password = "JDJ57TE655L";   /* personal network password */

/*MQTT*/
const char* mqtt_server = "broker.mqttdashboard.com";
const char* moisture_topic = "field1/plot1/soilHumidity";
const char* mqtt_username = "smart-agro-user-123";
const char* mqtt_password = "test-password@smartAgro";
const char* mqtt_clientID = "smart-agro-test-client-publish";
int mqtt_port = 1883; /*listener port for the broker*/

/*Initialize the WiFi and MQTT client objects*/
WiFiClient wifiClient;
PubSubClient mqttClient(mqtt_server, mqtt_port, wifiClient);
/* PubSubClient (server, port, [callback], client, [stream])*/

/*Set up WiFi Connection*/
void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /*connect to the WiFi*/
  WiFi.begin(ssid, wifi_password);

  /*Wait until the connection has been confirmed before continuing*/
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*Debugging - Output the IP Address of the ESP32*/
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/*Set up MQTT Connection*/
void setupMQTT() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);

  /* Connect to MQTT Broker
     client.connect returns a boolean value to let us know if the connection was successful.
     boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
  */

  if (mqttClient.connect(mqtt_clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
    // Subscribe or resubscribe to a topic here. can subscribe to more topics.
    // client.subscribe("esp8266/output");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
    mqttClient.state(); // will provide more information on why it failed.
  }
}

/*create the callback function to process the received message.*/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  String tempPayload;

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    tempPayload += (char)payload[i];
  }
  Serial.println();
}

/*Reconnect to MQTT Broker if connection failed*/
void reconnect() {
  // Loop until reconnected
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");

    // Attempt to connect
    if (mqttClient.connect(mqtt_clientID, mqtt_username, mqtt_password)) {
      Serial.println("Connected");
      // Subscribe or resubscribe to a topic here. can subscribe to more topics.
      // client.subscribe("esp8266/output");

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println("trying again in 5 seconds");
      delay(5000);
    }
  }
}

/*Get sensor readings*/
int getSensorReading() {
  int sensorValue = analogRead(sensor_pin);
  int moisturePercentage = map(sensorValue, 1400, 4095, 100, 0);
  Serial.println(moisturePercentage);
  return moisturePercentage;
}

void publishMessage() {
  /*
    Publish using the publish method.
    There are four options all return an Integer, 1 for success (true) and 0 for fail(false).

    These two use a character array for the payload:
        publish (topic, payload)
        publish (topic, payload, retained)

    These two use a Byte Array for the payload:
        publish (topic, payload, length)
        publish (topic, payload, length, retained)
  */

  int moistureReading = getSensorReading();

  /* MQTT can only transmit strings */

  /*PUBLISH to the MQTT Broker*/
  if (mqttClient.publish(moisture_topic, String(moistureReading).c_str())) {
    Serial.println("Sensor Data Sent!");
  }
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    mqttClient.connect(mqtt_clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    mqttClient.publish(moisture_topic, String(moistureReading).c_str());
  }

  /*For a different additional Topic copy and edit the same code snippet above*/
}

void setup() {
  Serial.begin(115200);
  delay(setupDelay);

  setupWifi();
  setupMQTT();

}


void loop() {

  if (!mqttClient.connected()) {
    reconnect();
  } else {
    mqttClient.loop();
    long now = millis();
    long previous_time = 0;

    // new readings are being published every 1 seconds
    if (now - previous_time > 1000) {
      previous_time = now;
    }
    publishMessage();
    delay(publishDelay);
  }

}
