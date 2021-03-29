#include <Arduino.h>
#include <mqtt.h>

#define ONE_MINUTE 1e6 * 60

void setup()
{
  Serial.begin(115200);
  pinMode(D0, WAKEUP_PULLUP);
  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);

  connectWifi();

  int sensorValue = analogRead(A0);
  int outputValue = map(sensorValue, 0, 1023, 0, 3300);
  Serial.printf("ADC %d\n", outputValue);
  send_mqtt_humidity(outputValue);

  digitalWrite(D7, LOW);
  delay(1000);

  ESP.deepSleep(ONE_MINUTE * 5);
}

void loop()
{
}