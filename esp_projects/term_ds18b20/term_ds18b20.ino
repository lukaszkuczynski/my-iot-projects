#include <OneWire.h>
#include <DallasTemperature.h>
#include "mqtt_communication.h"

const int oneWireBus = 4;     

const int MIN_VOLTAGE = 2600;
const int SLEEP_TIME = 1e6 * 60 * 1;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);  

void setup() {
  Serial.begin(115200);
  sensors.begin();
  delay(1000);
  int voltage = measureSupplyVoltage();
  if (voltage < MIN_VOLTAGE) {
    Serial.println("Voltage too Low, will sleep forever");
    ESP.deepSleep(0);
  }

  setup_wifi();
  float temperatureCelsius = measureTemperature();
  sendTemp("device1", temperatureCelsius, voltage); 
  delay(1000);
//  ESP.deepSleep(SLEEP_TIME);
}

float measureTemperature() {
  int numberOfDevices = sensors.getDeviceCount();
  Serial.print("Found devices ");
  Serial.println(numberOfDevices);
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
  Serial.println("mV");
  return voltageMv;
}

void loop() {
  
  measureTemperature();
  delay(1000);
}
