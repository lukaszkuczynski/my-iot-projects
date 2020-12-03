#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "my_local_wifi.h"
#include <ESP8266WiFi.h>

#define MQTT_SERVER "192.168.0.171"
#define MQTT_SERVERPORT 1883

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT);
Adafruit_MQTT_Publish door_publish_channel = Adafruit_MQTT_Publish(&mqtt, "/bedroom/wardrobe_door");
Adafruit_MQTT_Publish power_publish_channel = Adafruit_MQTT_Publish(&mqtt, "/bedroom/power");

void connectWifi()
{
    //connect to your local wi-fi network
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    //check wi-fi is connected to wi-fi network
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected..!");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP());
}

void send_mqtt_door_state(int value);

void MQTT_re_connect()
{
    int8_t ret;

    if (mqtt.connected())
    {
        return;
    }
    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0)
    { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
        retries--;
        if (retries == 0)
        {
            // basically die and wait for WDT to reset me
            while (1)
                ;
        }
    }
    send_mqtt_door_state(-2);
    Serial.println("MQTT Connected!");
}

void send_mqtt_door_state(int value)
{
    MQTT_re_connect();
    door_publish_channel.publish(value);
}

void send_mqtt_voltage(double value)
{
    MQTT_re_connect();
    power_publish_channel.publish(value);
}