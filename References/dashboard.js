// using mqtt.js library

const express = require("express");
const mqtt = require("mqtt");


/*----------------------------- Setting up MQTT --------------------------------*/

// const host: 'localhost';
// const port: 1883;
// const protocol: mqtt;
// const path: mqtt;

// const broker = '${protocol}://${host}:${port}/${path]}';

// ws, mqtt (non-encrypted), mqtts, wss (SSL encrypted)

const host = 'ws://broker.emqx.io:8083/mqtt';

const options = {
  protocolId: 'MQTT',
  // protocol version - default 4 (v3.1.1) - 3 (v3.1) and 5 (v5.0)
  protocolVersion: 4,
  // keepalive - default = 60 seconds. 0 means disabled
  keepalive: 60,
  // Clean session. default is true
  clean: true,
  // reconnection - default is 1000 milliseconds. 0 means automatic reconnect disabled
  reconnectPeriod: 1000,
  // connectTimeout - waiting time before receiving CONNACK. default - 30000 milliseconds
  connectTimeout: 30000,
  // Authentication
  clientId: 'testClient', // default - 'mqttjs_' + Math.random().toString(16).substr(2, 8)
  username: 'myuser',
  password: 'mypassword',
  // last will message
  will: {
    topic: 'willMsg',
    payload; 'Connection Closed abnormally..!',
    qos: 1,
    retain: true
  }
}

console.log('Connecting mqtt client...');

const client = mqtt.connect(broker, options);


/*--------------------------- MQTT client events ------------------------------*/

client.on('connect', function (connack) {
  console.log('connection successful');
  console.log('Connected with CONNACK: ' + connack);
});

client.on('reconnect', function () {
  console.log('Reconnecting...')
});

client.on('close', function () {
  console.log('Disconnected')
});

client.on('offline', function () {
  console.log('client offline')
});

client.on('error', function (error) {
  console.log('ERROR! client cannot connect successfully');
  console.log('error:' + error);
  // client.end();
});

client.on('message', function (topic, payload, packet) {
  // Payload is Buffer
  console.log('client received a published Payload');
  console.log(`Topic: ${topic}, Message: ${payload.toString()}, QoS: ${packet.qos}`)
});

// client.on('connect', function () {
//   console.log('Connected')
//   client.subscribe('test', function (err) {
//     if (!err) {
//       client.publish('test', 'Hello mqtt')
//     }
//   })
// })
// client.on('message', function (topic, message) {
//   // message is Buffer
//   console.log(message.toString())
//   client.end()
// })


/*--------------------------- MQTT client functions ------------------------------*/

// Client.publish(topic, message, [options], [callback])
client.publish('testtopic', 'Hello, MQTT!', { qos: 0, retain: false }, function (error) {
  if (error) {
    console.log(error)
  } else {
    console.log('Published')
  }
});


//Client.subscribe(topic/topic-array/topic-object, [options], [callback])
// topic object - {'test1': {qos: 0}, 'test2': {qos: 1}}
// Granted is an array of {topic, QoS}
client.subscribe('testtopic', { qos: 0 }, function (error, granted) {
  if (error) {
    console.log(error)
  } else {
    console.log(`${granted[0].topic} was subscribed`)
  }
});


// Client.unsubscribe(topic/topic array, [options], [callback])
client.unsubscribe('testtopic', function (error) {
  if (error) {
    console.log(error)
  } else {
    console.log('Unsubscribed')
  }
});


// close a client
// Client.end([force], [options], [callback])
client.end();



app.get("/", function(req, res){
  console.log(mqtt);
});



app.listen(3000, function() {
  console.log("Server started on port 3000");
});




/*
  ------------------------cloud mqtt brokers and ports--------------------------

  MQTT.js supports
    - mqtt://localhost, on 1883 port.
    - mqtts://localhost, on 8884 port.

  test.mosquitto.org (Mosquitto)
      port - 1883
      Un-encrypted - port 8080 - for websockets
      Encrypted - port 8081 - for websockets over SSL
      SSL support - 8883 With Client certificate 8884

  iot.eclipse.org (Mosquitto)
      support only encrypted and uses ports 80 and 443 for websockets over SSL.
      SSl support - 8883

  broker.hivemq.com (HiveMQ)
      port - 1883
      port 8000 - for websockets. Websockets over SSL not supported.
*/
