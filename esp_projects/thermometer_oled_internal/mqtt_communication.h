#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include "secrets.h"
#include <ArduinoJson.h>

const char* mqtt_server = "192.168.100.10";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    #ifdef ESP32
      WiFi.disconnect();
      WiFi.reconnect();
    #endif
    Serial.print(WiFi.status());
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp8266_temp")) {
      Serial.println("connected");
      // Subscribe
      // client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      // Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void sendTelemetry(float temperature, float pressure, float humidity, int voltage, String warning) {
  char msg[500];
  if (!client.connected()) {
    reconnect();
  }
  String deviceId = SENSOR_NAME;
  StaticJsonDocument<200> doc;
  doc["temp"] = temperature;
  doc["voltage"] = voltage;
  doc["device"] = deviceId;
  doc["pressure"] = pressure;
  doc["humidity"] = humidity;
  serializeJson(doc, msg);
  String channelName = "lukmqtt/sensor/"+deviceId;
  Serial.println("Sending to channel named "+channelName);
  client.publish(channelName.c_str(), msg);

  if (warning != "") {
    char msgWarning[200];
    StaticJsonDocument<200> docWarning;
    docWarning["warning"] = warning;
    serializeJson(docWarning, msgWarning);
    String channelName = "lukmqtt/sensor/warning/"+deviceId;
    Serial.println("Sending to channel named "+channelName);
    client.publish(channelName.c_str(), msgWarning);
  }
}

void sendMeasurements(String deviceId, float temp, int voltage, float lux) {
  char msg[200];
  if (!client.connected()) {
    reconnect();
  }
  StaticJsonDocument<200> doc;
  doc["temp"] = temp;
  doc["voltage"] = voltage;
  doc["device"] = deviceId;
  #ifdef LIGHT_SENSOR
  doc["light"] = lux;
  #endif
  serializeJson(doc, msg);
  Serial.println(msg);
  String channelName = "lukmqtt/sensor/"+deviceId;
  Serial.println("Sending to channel named "+channelName);
  client.publish(channelName.c_str(), msg);
}
