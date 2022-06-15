/* Smart Agro - final draft
   PubSubClinet library - https://pubsubclient.knolleary.net/
   PubSubClient API Documentation: https://pubsubclient.knolleary.net/api.html#disconnect

   Debbuging help: PubSubClient is not fully compatible with the ESP32
   Command to start the Mosquitto console in the Raspberry Pi: - sudo mosquitto
*/

// to edit - add deep sleep
// add a publish function and take code out of the void loop():

#include <PubSubClient.h> // Connect and publish to the MQTT broker
#include <WiFi.h> // Enables the ESP32 to connect to the local network (via WiFi)

#define sensor_pin 36 // sensor Pin 

/*Get sensor readings*/
int getReading() {
  int sensorValue = analogRead(sensor_pin);
  int moisturePercentage = map(sensorValue, 1400, 4095, 100, 0);
  Serial.println(moisturePercentage);
  return moisturePercentage;
}


/*WiFi*/
const char* ssid = "XXXXXXXXX";              // personal network SSID
const char* wifi_password = "XXXXXXXXXXX";   // personal network password

/*MQTT*/
const char* mqtt_server = "192.168.X.XXX";   // IP address of the broker
const char* moisture_topic = "field1/plant1/moisture";
const char* mqtt_username = "oshani";
const char* mqtt_password = "testMQTT1540";
const char* mqtt_clientID = "ESPclient_moisture";
int mqtt_port = 1883; /*listener port for the broker*/

/*Initialize the WiFi and MQTT client objects*/
WiFiClient wifiClient;
PubSubClient mqttClient(mqtt_server, mqtt_port, wifiClient);
/* to create an Instance of the client object the API lists several constructors
   PubSubClient (server, port, [callback], client, [stream])
*/
// long lastMsg = 0;
// char msg[50];
// int value = 0;

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

  /* Connect to MQTT Broker */
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password
  // boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])

  if (mqttClient.connect(mqtt_clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
    mqttClient.state();
    // will provide more information on why it failed.
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
  // Loop until we're reconnected
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");

    // Attempt to connect
    if (mqttClient.connect(mqtt_clientID, mqtt_username, mqtt_password)) {
      Serial.println("Connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more outputs)
      // client.subscribe("esp8266/output");

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*Deep Sleep*/
//void deepSleep() {
//  // Time to sleep
//  uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
//  // sleep for 30 minutes = 1800 seconds
//  uint64_t TIME_TO_SLEEP = 1800;
//  // enable timer deep sleep
//  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//  Serial.println("Going to sleep now");
//  // start deep sleep for 3600 seconds (60 minutes)
//  esp_deep_sleep_start();
//}

void setup() {
  Serial.begin(115200);
  delay(2000);

  setupWifi();
  setupMQTT();
  //  function for publishing the message;
  //  deepSleep();
}


void loop() {
  // sleeping so wont get here

  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  long now = millis();
  long previous_time = 0;

  // new readings are being published every 1 seconds
  if (now - previous_time > 1000) {
    previous_time = now;

   int moistureReading = getReading();
    // MQTT can only transmit strings
    // String moistureSensor = "Moisture: " + String((int)moistureReading + " % ";


    /*PUBLISH to the MQTT Broker (topic = moisture)*/
    
    if (mqttClient.publish(moisture_topic, String(moistureReading).c_str())) {
    Serial.println("Sensor Data Sent!");
    }
    // Again, client.publish will return a boolean value depending on whether it succeded or not.
    // If the message failed to send, we will try again, as the connection may have broken.
    else {
     // Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
     mqttClient.connect(mqtt_clientID, mqtt_username, mqtt_password);
     delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
     mqttClient.publish(moisture_topic, String(moistureReading).c_str());
    }

    // You need to convert the float variable to a char array,
    // so that you can publish the sensor reading in the "moisture_topic" topic:
    //    char mois_string[8];
    //    dtostrf(moistureReading, 1, 2, mois_string);
    //    Serial.print("Moisture: ");
    //    Serial.println(mois_string);
    //    mqttClient.publish(moisture_topic, mois_string);
    delay(2000);
  }
}



//void publishMessage() {
//  /*
//    Publish using the publish method.
//    There are four options all return an Integer, 1 for success (true) and 0 for fail(false).
//
//    These two use a character array for the payload:
//        publish (topic, payload)
//        publish (topic, payload, retained)
//
//    These two use a Byte Array for the payload:
//        publish (topic, payload, length)
//        publish (topic, payload, length, retained)
//  */
//
//  /*PUBLISH to the MQTT Broker (topic = moisture)*/
//  if (client.publish(moisture_topic, String(moisturePercentage).c_str())) {
//    Serial.println("Sensor Data Sent!");
//  }
//  // Again, client.publish will return a boolean value depending on whether it succeded or not.
//  // If the message failed to send, we will try again, as the connection may have broken.
//  else {
//    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
//    client.connect(mqtt_clientID, mqtt_username, mqtt_password);
//    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
//    client.publish(moisture_topic, String(moisturePercentage).c_str());
//  }
//
//  /*For a different additional Topic copy and edit the same code snippet above*/
//
//
//  boolean rc = mqttClient.publish("test", "This is a test message");
//  // boolean publish (topic, payload, [length], [retained])
//}
