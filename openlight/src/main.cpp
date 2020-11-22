#include <Arduino.h>

#define RELAY_PIN 5       // nodemcu v2
#define REED_SENSOR_PIN 4 // nodemcu v2

// #define RELAY_PIN 1       // for ESP-01
// #define REED_SENSOR_PIN 3 // for ESP-01

void updateLEDstatusAsToDoorStatus()
{
  int reedSensorValue = digitalRead(REED_SENSOR_PIN);
  bool isDoorOpen = reedSensorValue == 0;
  if (isDoorOpen)
  {
    Serial.println("Door opened, let us turn on the light");
    // Normally Open configuration, send LOW signal to let current flow
    digitalWrite(RELAY_PIN, LOW);
  }
  else
  {
    Serial.println("Door closed, lights down!");
    digitalWrite(RELAY_PIN, HIGH);
  }
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
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(REED_SENSOR_PIN), detectsDoorChange, CHANGE);
}

void loop()
{
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
  // digitalWrite(LED_BUILTIN, HIGH); // only for espo01

  delay(100);
}