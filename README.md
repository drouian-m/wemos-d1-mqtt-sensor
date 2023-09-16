# wemos-d1-mqtt-sensor
Wemos D1 Mini sensor controller with MQTT events publisher

## Description

The code of this project allows to configure a wemos d1 mini equipped with a temperature and humidity sensor DTH22 in Home Assistant via MQTT.

## Configuration

Fill the configuration variable sections with your informations.

```c
// Wifi connection
const char* ssid = "Wifi SSID";
const char* password = "Wifi password";

// MQTT
const char* mqtt_server = "127.0.0.1"; // Update the IP with your MQTT server
// uncomment following lines if your server is authenticated
// const char* mqtt_user = "user";
// const char* mqtt_password = "pass";

// MQTT Sensors
const char* sensor_uid = "sensor id";
const char* topic = "mqtt topic";
const char* sensor_location = "location";

// Home assistant discovery
const char* temperature_discovery_topic = "homeassistant/sensor/home_sensor_xxxx/temperature/config";
const char* temperature_readable_name = "YOURSENSORNAME temperature";
const char* humidity_discovery_topic = "homeassistant/sensor/home_sensor_xxxx/humidity/config";
const char* humidity_readable_name = "YOURSENSORNAME humidity";
```

If the wifi and MQTT configuration is correct, temperature and humidity sensors should appear in home assistant.

## Screenshots

![sensors](assets/sensors.png)

![add](assets/add.png)

![home](assets/homeview.png)

![graph](assets/graph.png)

## Resources

- https://github.com/MarkJB/wemos-d1-mini-esp8266-mqtt-sht30-temp-sensor
- https://roelofjanelsinga.com/articles/mqtt-discovery-with-an-arduino/