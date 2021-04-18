#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

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
    if (client.connect("ESP32_Client", "rpi", MQTT_PASSWORD)) {
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

void sendAhead(float aheadDistance) {
  if (!client.connected()) {
    reconnect();
  }
  client.publish("esp32/counter", "ahead");
}

void sendLocation(float distanceMap[]) {
  if (!client.connected()) {
    reconnect();
  }
  client.publish("esp32/counter", "location");
}