void checkSensors(){
  currMillis = millis();
  if (currMillis - prevMillisSensors >= CHECK_SENSORS_INTERVAL){
  StaticJsonDocument<50> mqttDoc;
  prevMillisSensors = currMillis;
  JsonObject mqttMsg = mqttDoc.to<JsonObject>();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    SensorError();
    return;
  }

  // Compute heat index in Fahrenheit and c
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  if (f != 2147483647 || h != 2147483647 || hif != 2147483647){
    mqttMsg["temp"] = f;
    mqttMsg["humidity"] = h;
    mqttMsg["indx"] = hif;
    //serializeJsonPretty(mqttMsg, Serial);
    char buffer[512];
    size_t n = serializeJson(mqttMsg, buffer);
    dataSend();
    Serial.println(buffer);
    client.publish(topic, buffer, n);
    } else {
      Serial.println(F("Invalid data from sensor"));
      mqttMsg["temp"] = 0;
      mqttMsg["humidity"] = 0;
      mqttMsg["indx"] = 0;
      char buffer[512];
      size_t n = serializeJson(mqttMsg, buffer);
      SensorError();
      client.publish(topic, buffer, n);
      }
  }
}
