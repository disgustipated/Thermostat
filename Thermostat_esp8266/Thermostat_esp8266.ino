#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <PubSubClient.h> //https://github.com/knolleary/pubsubclient
#include "DHT.h"
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeAlarms.h> 

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define mqtt_server "6.13.0.120"
#define mqtt_user "bedroom"
#define mqtt_password "bedroom"
#define topic "home/bedroom/temperature"

const int  statusPagePort = 8266;
const String SOFTWARE_VERSION = "1.0 Bedroom Thermostat";
const char* DEVICENAME = "BedroomThermostat"; 
//pins
const int WIFI_INFO_LED_PIN = 2;
const int WIFI_RESET_PIN = 14;
const long CHECK_WIFI_INTERVAL = 30000;
const long CHECK_MQTT_INTERVAL = 30000;
const long CHECK_SENSORS_INTERVAL = 5000;

unsigned long prevMillisSensors = 0;
unsigned long prevMillisWiFi = 0;
unsigned long prevMillisMQTT = 0;
unsigned long currMillis = millis();

MDNSResponder mdns;
ESP8266WebServer server(statusPagePort); //this cant be running on port 80 because of the wifimanager
WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(WIFI_RESET_PIN,INPUT_PULLUP);
  pinMode(WIFI_INFO_LED_PIN,OUTPUT);
  //wifiManager.setSTAStaticIPConfig(IPAddress(6,13,0,218), IPAddress(6,13,0,1), IPAddress(255,255,255,0)); //Remove this for DHCP
  wifiManager.autoConnect("ESPSetup", "Setup1");
  client.setServer(mqtt_server, 1883);
  dht.begin();
  setupWeb();
}

//main loop
void loop() {
  // wire a button with a 10k resistor to ground and the other end to pin 14 for resetting and to prompt for new network
  if ( digitalRead(WIFI_RESET_PIN) == LOW ) {
    WiFiManager wifiManager;
    wifiManager.setTimeout(600);
    WiFi.mode(WIFI_STA);
    wifiManager.startConfigPortal("OnDemandAP");
    Serial.println("Autoconnect portal running");
  }
  //checking if mqtt is connected
  checkMQTT();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  checkSensors();
  server.handleClient();
  Alarm.delay(100);
}
