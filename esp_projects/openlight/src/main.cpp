#include <Arduino.h>
#include "mqtt.h"

#define RELAY_PIN 5       // nodemcu v2 GPIO D1
#define REED_SENSOR_PIN 4 // nodemcu v2 GPIO D2

// #define RELAY_PIN 1       // for ESP-01
// #define REED_SENSOR_PIN 3 // for ESP-01

ADC_MODE(ADC_VDD);

void updateLEDstatusAsToDoorStatus()
{
  int reedSensorValue = digitalRead(REED_SENSOR_PIN);
  bool isDoorOpen = reedSensorValue == 0;
  if (isDoorOpen)
  {
    Serial.println("Door opened, let us turn on the light");
    digitalWrite(RELAY_PIN, LOW);
  }
  else
  {
    Serial.println("Door closed, lights down!");
    digitalWrite(RELAY_PIN, HIGH);
  }
  send_mqtt_door_state(isDoorOpen);
}

ICACHE_RAM_ATTR void detectsDoorChange()
{
  Serial.println("DoorStatus changed");
  updateLEDstatusAsToDoorStatus();
}

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(REED_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(REED_SENSOR_PIN), detectsDoorChange, CHANGE);
  connectWifi(10);
  MQTT_re_connect();
  send_mqtt_door_state(-1);
}

void loop()
{
  uint16_t voltage = ESP.getVcc();
  double dvcc = (float)voltage / 1024;
  Serial.printf("\nSending power value %.2f", dvcc);
  send_mqtt_voltage(dvcc);
  delay(60000);
}