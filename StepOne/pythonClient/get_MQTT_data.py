# Create a python file for subscribing to MQTT data -   sudo nano get_MQTT_data.py
# To Run the MQTT subscriber on RPi -                   python get_MQTT_data.py

import paho.mqtt.client as mqtt

MQTT_ADDRESS = '192.168.8.102'
MQTT_USER = 'oshani'
MQTT_PASSWORD = 'testMQTT1540'
MQTT_TOPIC = 'ESP32/YL69/moisture'


def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)

# variable rc holds an error code if the connection is not successful so the debugging is easier
#           0: Connection successful
#           1: Connection refused – incorrect protocol version
#           2: Connection refused – invalid client identifier
#           3: Connection refused – server unavailable
#           4: Connection refused – bad username or password
#           5: Connection refused – not authorized
#           6-255: Currently unused.


def on_message(client, userdata, msg):
    """ The callback for when a PUBLISH message is received from the server.
        expect that on this point current sensor value is obtained and printed to the terminal. """
    print(msg.topic + ' ' + str(msg.payload))


def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
