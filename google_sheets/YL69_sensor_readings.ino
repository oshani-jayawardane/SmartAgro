/*
 * Smart Agro
 * EESP32 publish sensor readings to google sheets
 * Using IFTTT 3rd party tool
 */
 
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <Wire.h>

const int sensor_pin = 36;

// Replace with your SSID and Password
const char* ssid     = "Dialog 4G";
const char* password = "JDJ57TE655L";

// Replace with your unique IFTTT URL resource
// go to IFTTT services page -> documentation
const char* resource = "https://maker.ifttt.com/trigger/YL69_sensor_readings/with/key/fyxe5ZkD2ZAGmcQaDRAMPWyzXoXgMn39P2kiVsfbfl8";

// To trigger an Event with an arbitrary JSON payload
// const char* resource = "https://maker.ifttt.com/trigger/YL69_sensor_readings/json/with/key/fyxe5ZkD2ZAGmcQaDRAMPWyzXoXgMn39P2kiVsfbfl8";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 30 minutes = 1800 seconds
uint64_t TIME_TO_SLEEP = 1800;

void setup() {
  Serial.begin(115200); 
  delay(2000);

  initWifi();
  makeIFTTTRequest();
    
  #ifdef ESP32
    // enable timer deep sleep
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
    Serial.println("Going to sleep now");
    // start deep sleep for 3600 seconds (60 minutes)
    esp_deep_sleep_start();
  #else
    // Deep sleep mode for 3600 seconds (60 minutes)
    Serial.println("Going to sleep now");
    ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); 
  #endif
}

void loop() {
  // sleeping so wont get here 
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

int getReading() {
  int sensorValue = analogRead(sensor_pin);
  int moisturePercentage = map(sensorValue,1400,4095,100,0);
  Serial.println(moisturePercentage);
  return moisturePercentage;
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  // Sensor Values

  // if you don't trigger the Event with an arbitrary JSON payload, just leave it as is:
     String jsonObject = String("{\"value1\":\"") + getReading() + "\"}";
  
  // String jsonObject = String("{\"value1\":\"") + getReading() + "\",\"value2\":\"" + getReading() + "\",\"value3\":\"" + getReading() + "\"}";
                      
  // if you don't trigger the Event with an arbitrary JSON payload, these codes might be helpful in  Javascript:
    //  let payload = JSON.parse(MakerWebhooks.jsonEvent.JsonPayload)
    //  let formattedRow = `${payload.value1}`
    //  GoogleSheets.appendToGoogleSpreadsheet.setFormattedRow(formattedRow)

                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}
