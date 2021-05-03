#include <OneWire.h>
#include <DallasTemperature.h>
#include "mqtt_communication.h"

const int oneWireBus = 5;     

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);  

void setup() {
  Serial.begin(115200);
  sensors.begin();
  setup_wifi();
}

float measureTemperature() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC"); 
  return temperatureC;
}

int measureSupplyVoltage() {
  int adcRead = analogRead(A0);
  int voltageMv = map(adcRead, 0, 1024, 0, 3300);
  Serial.print(voltageMv);
  Serial.println("mv");
  return voltageMv;
}

void loop() {
  float temperatureCelsius = measureTemperature();
  int voltage = measureSupplyVoltage();
  sendTemp("device1", temperatureCelsius, voltage); 
  delay(1000);
}
