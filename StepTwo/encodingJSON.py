import paho.mqtt.client as mqtt
import time
import json

brokers_out = {"broker1":"192.168.8.102",
               "broker2":"test.mosquitto.org",
               "broker3":"iot.eclipse.org"}

print(brokers_out)
print("\nbrokers_out is a ", type(brokers_out))
print("\nbroker 1 address = ", brokers_out["broker1"])
data_out = json.dumps(brokers_out) #encode object to JSON
print("\nConverting to JSON\n")
print("\ndata-type ", type(data_out))
print("\ndata out =", data_out)
# at reciever
print("\nRecieved Data\n")
data_in = data_out
print("\ndata-type", type(data_in))
print("\ndata in =", data_in)
brokers_in = json.loads(data_in) #convert incoming JSON to object
print("\nbrokers_in is a ", type(brokers_in))
print("\n\nbroker 2 address =", brokers_in["broker2"])
cont = input("\nenter to continue")
###########
###########

def on_message(client, userdata, msg):
    topic = msg.topic
    m_decode = str(msg.payload.decode("utf-8","ignore"))
    print("\n\ndata recieved type", type(m_decode))
    print("\ndata recieved", m_decode)
    print("\nConverting from JSON to object")
    m_in = json.loads(m_decode)
    print(type(m_in))
    print("\nbroker 2 address = ", m_in["broker2"])

topic = "test/json_test"
client = mqtt.Client("pythontest1")
client.on_message = on_message
print("\n\nConnecting to broker ", brokers_out["broker2"])
client.connect(brokers_out["broker2"])
client.loop_start()
client.subscribe(topic)
time.sleep(3)
print("\n\nSending Data....")
