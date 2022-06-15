# SmartAgro
## Step 01
### Code for YL69 soil moisture sensor in Arduino IDE using ESP32
https://github.com/oshani-jayawardane/SmartAgro/blob/main/StepOne/YL69moistureSensor.ino
## Step 02
### Publish sensor readings to Node-Red dashboard via MQTT (HiveMQ cloud broker)
// add code
### Publish sensor readings to Node-Red dashboard via MQTT (mosquitto broker installed on raspberry pi)
**using AsynMQTTclient**
https://github.com/oshani-jayawardane/SmartAgro/blob/main/StepOne/YL69moistureSensorMQTT.ino <br />
**using PubSubclient**
[https://github.com/oshani-jayawardane/SmartAgro/blob/main/StepOne/YL69moistureSensorMQTT_ex2.ino ](https://github.com/oshani-jayawardane/SmartAgro/tree/main/StepOne/pythonClient)<br /><br />

how to install mosquitto on raspberry pi: <br />
https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/ <br />
testing mosquitto on raspberry pi: <br />
https://randomnerdtutorials.com/testing-mosquitto-broker-and-client-on-raspbbery-pi/#more-31721 <br />
Node-RED on Raspberry Pi <br />
https://randomnerdtutorials.com/getting-started-with-node-red-on-raspberry-pi/ <br />
Node-RED Dashboard on Raspberry Pi <br />
https://randomnerdtutorials.com/getting-started-with-node-red-dashboard/ <br />

### Final code to publish ESP32 data to broker
https://github.com/oshani-jayawardane/SmartAgro/blob/main/StepTwo/ESP32_to_broker.ino <br />

## Step 03 - MQTT over websockets using Javascript client
MQTT websockets javascript docs - https://www.eclipse.org/paho/files/jsdoc/Paho.MQTT.Client.html <br />
Javascript Client MQTT - Namespace: https://www.eclipse.org/paho/files/jsdoc/Paho.MQTT.html <br />
**Access Client Class and Message Class documentations through this** <br />

## Step 04 - Testing Databases

## Additional Step - ESP32 Publish Sensor Readings to Google Sheets
Integrating directly with Google Sheets requires an HTTPS authentication - use a 3rd party service like IFTTT <br />
Steps: <br />
1. Connect the ESP to the Wi-Fi network <br />
2. Obtain sensor readings <br />
3. Allow ESP32 to communicate with the IFTTT Webhooks service that publishes the readings to a spreadsheet on Google Sheets saved in a Google Drive’s folder <br />
4. After publishing the readings, let the ESP go into deep sleep mode for 30 minutes <br />
5. After 30 minutes the ESP wakes up <br />
6. After waking up, the ESP connects to Wi-Fi, and the process repeats <br /><br />

**Code - https://github.com/oshani-jayawardane/SmartAgro/blob/main/google_sheets/YL69_sensor_readings.ino** <br />
**IFTTT google drive link - https://drive.google.com/drive/folders/1FEcY7AJZDSFRBqjVrAc7cJgmkN55QRmr?usp=sharing** <br />
**IFTTT Applet - https://ifttt.com/applets/ZPQpDnhf/edit**<br /><br />

reference: https://randomnerdtutorials.com/esp32-esp8266-publish-sensor-readings-to-google-sheets/ <br />

## Additional Step - Building the complete IoT solution using Amazon Web Services
https://github.com/oshani-jayawardane/SmartAgro/tree/main/AWS <br />
