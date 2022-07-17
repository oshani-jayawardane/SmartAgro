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

  let connectionMessage = 'broker connection status';

  res.render("dashboard", {
    subTopic: stopic,
    field: field,
    plot: plot,
    sensor: sensor,
    connectionMessage: connectionMessage,
    moistureMessage: 'moisture %'
  });

});


/*-------------------------------- Server ------------------------------------*/

app.listen(3000, function() {
  console.log("Server started on port 3000");
});
