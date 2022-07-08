const int sensor_pin = 36;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(sensor_pin);
  //Serial.println(sensorValue);

  //int moisturePercentage = map(sensorValue,350,1023,100,0);
  int moisturePercentage = map(sensorValue,1400,4095,100,0);
  Serial.println(moisturePercentage);

  delay(1000);

}
