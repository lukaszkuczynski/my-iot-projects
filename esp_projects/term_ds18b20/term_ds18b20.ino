#include <OneWire.h>
#include <DallasTemperature.h>
#include "mqtt_communication.h"

ADC_MODE(ADC_VCC);

const int oneWireBus = 4;     

const int POWER_SENSOR_PIN = 5;

const int MIN_VOLTAGE = 2600;
const int SLEEP_TIME = 1e6 * 60 * 5;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);  

void setup() {
  Serial.begin(115200);
  pinMode(POWER_SENSOR_PIN, OUTPUT);
  delay(500);
  int voltage = measureSupplyVoltage();
  if (voltage < MIN_VOLTAGE) {
    Serial.println("Voltage too Low, will sleep forever");
    ESP.deepSleep(0);
  }
  setup_wifi();
  float temperatureCelsius = measureTemperature();
  sendTemp("temperature_living_room", temperatureCelsius, voltage); 
  delay(1000);
  ESP.deepSleep(SLEEP_TIME);
}

float measureTemperature() {
  digitalWrite(POWER_SENSOR_PIN, HIGH);
  sensors.begin();
  delay(500);  
  int numberOfDevices = sensors.getDeviceCount();
  Serial.print("Found devices ");
  Serial.println(numberOfDevices);
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC"); 
  digitalWrite(POWER_SENSOR_PIN, LOW);
  return temperatureC;
}

int measureSupplyVoltage() {
  float voltageMv = ESP.getVcc();
  Serial.print(voltageMv);
  Serial.println("mV");
  return voltageMv;
}

void loop() {
}
