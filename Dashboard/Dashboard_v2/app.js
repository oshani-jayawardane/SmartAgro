// using mqtt.js library

const express = require("express");
const ejs = require("ejs");
const bodyParser = require("body-parser");
const mqtt = require("mqtt");
const mongoose = require("mongoose");
const lodash = require("lodash");

const app = express();
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({extended:true}));
app.use(express.static("public"));


/*----------------------------- Setting up MQTT --------------------------------*/

// const host: 'localhost';
// const port: 1883;
// const protocol: mqtt;
// const path: mqtt;

// const broker = '${protocol}://${host}:${port}/${path]}';

const broker = 'ws://broker.emqx.io:8083/mqtt';

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
    payload: 'Connection Closed abnormally..!',
    qos: 1,
    retain: true
  }
}

console.log('Connecting mqtt client...');

const client = mqtt.connect(broker, options);

const connected_flag = 0;

/*-------------------------------- Homepage ----------------------------------*/

app.get("/", function(req, res){
  res.render("home");
});


/*-------------------- Loading Dashboard with Topic Page ---------------------*/

app.get("/topic", function(req, res){
  res.render("topic");
});

// app.post("/topic", function(req,res){
//
//   const subTopic = req.body.stopic;
//   res.redirect("/");
//
// });


/*-------------------------------- Dashboard ----------------------------------*/

app.get("/dashboard", function(req, res){

  res.render("dashboard", {
    subTopic: 'topic',
    moistureMessage: 'moisture %'
  });

  client.on('connect', function (connack) {
    console.log('successfully Connected with CONNACK:', connack);
    connected_flag = 1;
    // res.send('Successfully connected to "broker.emqx.io" on port 8083');

    client.subscribe('testtopic', { qos: 1 }, function (error, granted) {
      if (!error) {
        console.log(`${granted[0].topic} was subscribed`);
      } else {
        console.log(error);
      }
    });

  });

  client.on('reconnect', function () {
    console.log('Reconnecting...');
    // res.send('Reconnecting to broker...');
  });

  client.on('message', function (topic, payload, packet) {
    console.log('client received a published Payload');
    res.send(`Topic: ${topic}, Message: ${payload.toString()}, QoS: ${packet.qos}`);
  });

});



/*-------------------------------- Server ------------------------------------*/

app.listen(3000, function() {
  console.log("Server started on port 3000");
});
