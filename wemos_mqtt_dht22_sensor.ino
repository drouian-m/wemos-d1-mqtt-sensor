#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define LED 2

StaticJsonDocument<200> doc;

// Configuration
// Sensor
uint8_t pin = D4;
DHT dht(pin, DHTTYPE);

unsigned long interval = 10 * 60000; // 10 minutes

// Offsets
long temp_offset = -4.8;
long hum_offset = 10;

// Wifi connection
const char* ssid = "Wifi SSID";
const char* password = "Wifi password";

// MQTT
const char* mqtt_server = "127.0.0.1"; // Update the IP with your MQTT server
// uncomment following lines if your server is authenticated
// const char* mqtt_user = "user";
// const char* mqtt_password = "pass";

// MQTT Sensors
String sensor_location = "location";
String sensor_uid = "wemos_" + sensor_location;
String topic = "sensor/temperature/" + sensor_location;

// Home assistant discovery
String temperature_discovery_topic = "homeassistant/sensor/home_sensor_" + sensor_location + "/temperature/config";
String temperature_readable_name = "YOURSENSORNAME " + sensor_location;
String humidity_discovery_topic = "homeassistant/sensor/home_sensor_" + sensor_location + "/humidity/config";
String humidity_readable_name = "YOURSENSORNAME " + sensor_location;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(sensor_uid.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      send_MQTT_discovery_sensors();
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(250);
    }
  }
}

void send_MQTT_discovery_msg(String discovery_topic, String name, String type, String unit, String value_mapping ) {
  DynamicJsonDocument doc(1024);
  char buffer[256];

  doc["name"] = name;
  doc["stat_t"] = topic;
  doc["unit_of_meas"] = unit;
  doc["dev_cla"] = type;
  doc["frc_upd"] = true;
  doc["val_tpl"] = value_mapping;

  size_t n = serializeJson(doc, buffer);
  client.publish(discovery_topic.c_str(), buffer, n);
}

void send_MQTT_discovery_sensors() {
  send_MQTT_discovery_msg(temperature_discovery_topic, temperature_readable_name, "temperature", "°C", "{{ value_json.temperature|round(1) }}");
  send_MQTT_discovery_msg(humidity_discovery_topic, humidity_readable_name, "humidity", "%", "{{ value_json.humidity|round(1) }}");
}

void send_temp() {
  float h = dht.readHumidity() + hum_offset;
  float t = dht.readTemperature() + temp_offset;

  // Values are printed on the serial console for debugging
  Serial.print("{\"humidity\": ");
  Serial.print(h);
  Serial.print(", \"temp\": ");
  Serial.print(t);
  Serial.print("}\n");

  if (t >= 0) {
    doc["location"] = sensor_location;
    doc["temperature"] = t;
    doc["humidity"] = h;
    char JSONmessageBuffer[200];
    serializeJson(doc, JSONmessageBuffer);
    client.publish(topic.c_str(), JSONmessageBuffer);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(LED, OUTPUT);
  pinMode(pin, INPUT);
  dht.begin();
  delay(5000);
}

void loop() {
  digitalWrite(LED, LOW);
  if (!client.connected()) {
    reconnect();
  }
  send_temp();
  client.loop();
  delay(interval);
}
