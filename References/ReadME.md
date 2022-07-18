# Available brokers and port numbers

  ------------------------cloud mqtt brokers and ports--------------------------<br/>

  **MQTT.js supports**<br/>
      mqtt://localhost, on 1883 port.<br/>
      mqtts://localhost, on 8884 port.<br/>

  **test.mosquitto.org (Mosquitto)**<br/>
      port - 1883<br/>
      Un-encrypted - port 8080 - for websockets<br/>
      Encrypted - port 8081 - for websockets over SSL<br/>
      SSL support - 8883 With Client certificate 8884<br/>

  **iot.eclipse.org (Mosquitto)**<br/>
      support only encrypted and uses ports 80 and 443 for websockets over SSL.<br/>
      SSl support - 8883<br/>

  **broker.hivemq.com (HiveMQ)**<br/>
      port - 1883<br/>
      port 8000 - for websockets. Websockets over SSL not supported.<br/>
      
  **broker.emqx.io (EMQX broker)**<br/>
      ws port - 8083<br/>
      wss secure port - 8084<br/>
      
  ws, mqtt (non-encrypted), mqtts, wss (SSL encrypted) <br/>

# EMQX broker
[MQTT5 Broker](https://www.emqx.com/en/mqtt/public-mqtt5-broker) <br/>

[Dashboard](http://tools.emqx.io/recent_connections) <br/>

[Connect to MQTT broker with Websocket](https://www.emqx.com/en/blog/connect-to-mqtt-broker-with-websocket) <br/>

[How to use MQTT in Node.js](https://www.emqx.com/en/blog/how-to-use-mqtt-in-nodejs) <br/>

# HiveMQ broker

open a cluster on your account on: https://console.hivemq.cloud/

connect to cluster from: http://www.hivemq.com/demos/websocket-client/

enter cluster provided URL and credentials
    
[HiveMQ Documentation](https://www.hivemq.com/docs/hivemq/4.8/user-guide/introduction.html)<br/>

[HiveMQ public broker](https://www.hivemq.com/public-mqtt-broker/) <br/>

[Getting started with HiveMQ Cloud Free and WebSockets - video](https://www.youtube.com/watch?v=2LZNsRMx8Wg) <br/>

[WebSocket Support for HiveMQ Cloud Basic](https://www.hivemq.com/blog/websocket-support-for-hivemq-cloud-basic/)<br/>

[HiveMQ - MQTT over Websockets](https://www.hivemq.com/blog/mqtt-over-websockets-with-hivemq/) <br/>

[MQTT Web Applications: How to build your own!](https://www.hivemq.com/blog/build-javascript-mqtt-web-application/)<br/>

# Reference Docs - Mqtt.js library - for Smart Agro by Oshani
https://github.com/oshani-jayawardane/SmartAgro/blob/main/References/dashboard.js

# MQTT.js library reference docs
  1. https://www.hivemq.com/blog/mqtt-client-library-mqtt-js/
  2. https://www.emqx.com/en/blog/mqtt-js-tutorial

# Paho Js client library reference docs
  1. https://www.eclipse.org/paho/index.php?page=clients/js/index.php 
  2. https://github.com/eclipse/paho.mqtt.javascript
  3. https://www.eclipse.org/paho/files/jsdoc/Paho.MQTT.Client.html 

# Reference Projects by steve

## [Display Dashboard](https://github.com/oshani-jayawardane/SmartAgro/blob/main/References/web-page-mqtt-display-dashbaord.htm)
MQTT over websockets - Paho Javascript Client

## [Control Dashboard](https://github.com/oshani-jayawardane/SmartAgro/blob/main/References/web-page-mqtt-control-dashbaord.htm)
MQTT over websockets - Paho Javascript Client





