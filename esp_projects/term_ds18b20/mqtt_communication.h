#include <ESP8266WiFi.h>
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

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp8266_temp", "rpi", MQTT_PASSWORD)) {
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

void sendTemp(String deviceId, float temp, int voltage) {
  char msg[200];
  if (!client.connected()) {
    reconnect();
  }
  StaticJsonDocument<200> doc;
  doc["temp"] = temp;
  doc["voltage"] = voltage;
  serializeJson(doc, msg);
  Serial.println(msg);
  String channelName = "esp8266/temperature/"+deviceId;
  client.publish(channelName.c_str(), msg);
}
