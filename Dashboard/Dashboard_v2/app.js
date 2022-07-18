
// TODO: use Ajax to dynamically load data on to the web page

// using mqtt.js library

const express = require("express");
const ejs = require("ejs");
const bodyParser = require("body-parser");
const mqtt = require("mqtt");
const mongoose = require("mongoose");
const _ = require("lodash");

const app = express();
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({
  extended: true
}));
app.use(express.static("public"));


let field = '';
let plot = '';
let sensor = '';
let stopic = ``;
let connection_flag = 0;
let connectionMessage = '';
let subscriptionMessage = '';
let publishMessage = '';
let packetMessage = '';


/*-------------------------------- Homepage ----------------------------------*/

app.get("/", function(req, res) {
  res.render("home");
});


/*-------------------- Loading Dashboard with Topic Page ---------------------*/

app.get("/topic", function(req, res) {
  res.render("topic");
});

app.post("/topic", function(req, res) {

  field = _.camelCase(req.body.field);
  plot = _.camelCase(req.body.plot);
  sensor = _.camelCase(req.body.sensor);

  console.log(field, plot, sensor);

  stopic = `${field}/${plot}/${sensor}`

  console.log(stopic);

  res.redirect("/dashboard");

});


/*-------------------------------- Dashboard ----------------------------------*/

app.get("/dashboard", function(req, res) {

  /*********** MQTT connection ***********/

  // const host = 'broker.hivemq.com';
  // const port = 8000;
  // const protocol: mqtt;
  // const path: mqtt;

  const host = 'broker.emqx.io';
  const port = 8083;
  const protocol = 'ws';
  const path = 'mqtt';

  const broker = `${protocol}://${host}:${port}/${path}`;

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
    username: 'oshani',
    password: 'testMQTT1540',
    // last will message
    will: {
      topic: 'willMsg',
      payload: 'Connection Closed abnormally..!',
      qos: 1,
      retain: true
    }
  }

  console.log('Connecting mqtt client...');

  const client = mqtt.connect(broker, options);

  // connect MQTT
  client.on('connect', function(connack) {
    console.log('successfully Connected with CONNACK:', connack);
    connection_flag = 1;

    // subscribe to topic
    client.subscribe(stopic, {
      qos: 0
    }, function(err) {
      if (!err) {
        subscriptionMessage = `subscribed to topic ${stopic}`;
        console.log(`subscribed to topic ${stopic}`);

        // publish to topic
        client.publish(stopic, 'Hello mqtt', { qos: 0 }, function (err){
          if(!err){
            publishMessage = `published to topic ${stopic}`;
            console.log(`published to topic ${stopic}`);
          } else {
            console.log(err);
          }
        }); // publish to topic

      }
    }); // subscribe to topic

  }); // connect MQTT

  // MQTT message
  client.on('message', function(topic, payload, packet) {
    console.log('client received a published Payload');
    console.log(`Topic: ${topic}, Message: ${payload.toString()}, QoS: ${packet.qos}`);
    packetMessage = `Recieved payload - Topic: ${topic}, Message: ${payload.toString()}, QoS: ${packet.qos}`;
  }); // MQTT message

  if (connection_flag == 1) {
    connectionMessage = 'successfully connected to broker emqx on port 8083';
  } else {
    connectionMessage = 'cannot connect to broker';
  }

  /*********** END MQTT connection ***********/

  /*********** Load Dashbaord ***********/

  res.render("dashboard", {
    connectionMessage: connectionMessage,
    subscriptionMessage: subscriptionMessage,
    publishMessage: publishMessage,
    packetMessage: packetMessage,
    field: _.lowerCase(field),
    plot: _.lowerCase(plot),
    sensor: _.lowerCase(sensor)
  });

});


/*-------------------------------- Server ------------------------------------*/

app.listen(3000, function() {
  console.log("Server started on port 3000");
});
