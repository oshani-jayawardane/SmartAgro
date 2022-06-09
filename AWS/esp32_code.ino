/*Debbuging help: PubSubClient is not fully compatible with the ESP32*/
/* In this code, WiFi connection is disconnected after the MQTT data is sent and reconnect after one hour.*/
/*Command to start the Mosquitto console in the Raspberry Pi: - sudo mosquitto*/

/*RPi - acting as both broker and subscriber*/

/*
  Steps:
    1. set up the sensor and the MQTT Broker to Receive MQTT Data
    2. set up the MQTT publisher (Add WiFi and enable MQTT to the MQTT Publisher)
    3. Check if MQTT Data is Received by Mosquitto
    4. Set up the subscriber to Receive Data from MQTT Broker
*/

/*
steps in the publisher setup:
    Establish a WiFi and MQTT connection.
    The sensor readings are stored as strings.
    The strings are sent via MQTT to the broker.
*/

#include <PubSubClient.h> // Connect and publish to the MQTT broker
#include <WiFi.h> // Enables the ESP32 to connect to the local network (via WiFi)

#define sensor_pin 36 // sensor Pin 

/*WiFi*/
const char* ssid = "Dialog 4G";              // Your personal network SSID
const char* wifi_password = "JDJ57TE655L";   // Your personal network password

/*MQTT*/
// char *mqttServer = "broker.mqttdashboard.com";
// const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";
const char* mqtt_server = "192.168.8.101";              // IP address of the broker
const char* moisture_topic = "field/ESP32/YL69";
const char* mqtt_username = "oshani";
const char* mqtt_password = "testMQTT1540";
const char* mqtt_clientID = "client_moisture";
int mqttPort = 1883; /*listener port for the broker*/

/*Initialize the WiFi and MQTT client objects*/
WiFiClient wifiClient;
PubSubClient client(mqtt_server, mqttPort, wifiClient);
// long lastMsg = 0;
// char msg[50];
// int value = 0;


/*Function to connect to the MQTT broker via WiFi*/
void connect_MQTT() {
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

  /* Connect to MQTT Broker */
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password
  
  if (client.connect(mqtt_clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

/*Function to connect to MQTT
void setup_MQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  connect_MQTT();
  Serial.setTimeout(2000);

  int sensorValue = analogRead(sensor_pin);
  int moisturePercentage = map(sensorValue, 1400, 4095, 100, 0);
  //Serial.println(moisturePercentage);

  Serial.print("MoistureLevel: ");
  Serial.print(moisturePercentage);
  Serial.println(" %");

  // MQTT can only transmit strings
  String moisture = "Moisture: " + String((float)moisturePercentage) + " % ";

  /*PUBLISH to the MQTT Broker (topic = moisture)*/ 
  if (client.publish(moisture_topic, String(moisturePercentage).c_str())) {
    Serial.println("Sensor Data Sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(mqtt_clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(moisture_topic, String(moisturePercentage).c_str());
  }

  /*For a different additional Topic copy and edit the same code snippet above*/

  client.disconnect();  // disconnect from the MQTT broker
  //delay(1000 * 60);     // print new values every 1 Minute
  delay(1000 * 60); 
}
