//#define ESP32 // to decide if using 8266 or esp32 board
#define ESP8266
//#define LIGHT_SENSOR 
#define SENSOR_NAME "temperature_living_room"

#include <OneWire.h>
#include <DallasTemperature.h>
#include "mqtt_communication.h"
#include <Wire.h>
#include <BH1750.h>

#ifdef ESP32
const int VCC_READ_PIN = 15;
#else
ADC_MODE(ADC_VCC);
#endif

const int oneWireBus = 15;     // D8

const int POWER_SENSOR_PIN = 12; // D6

const int MIN_VOLTAGE = 2600;
const int SLEEP_TIME = 1e6 * 60 * 5;
//const int SLEEP_TIME = 1e6 * 5;
  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);  
BH1750 lightMeter;


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
  #ifdef LIGHT_SENSOR
  float lux = measureLight();
  #else 
  float lux = 0;
  #endif
  float temperatureCelsius = measureTemperature();
//  float temperatureCelsius = 0;
  sendMeasurements(SENSOR_NAME, temperatureCelsius, voltage, lux); 
  delay(1000);
  ESP.deepSleep(SLEEP_TIME);
}

float measureTemperature() {
  digitalWrite(POWER_SENSOR_PIN, HIGH);
  sensors.begin();
  delay(1000);  
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

float measureLight() {
  digitalWrite(POWER_SENSOR_PIN, HIGH);
  delay(1000);    
  Wire.begin();
  lightMeter.begin();
  float lux = lightMeter.readLightLevel();
  Serial.print("Light Meter: ");
  Serial.print(lux);
  Serial.println(" lx");
  digitalWrite(POWER_SENSOR_PIN, LOW);
  return lux;
}

int measureSupplyVoltage() {
  #ifdef ESP32
  int analogReadVal = analogRead(VCC_READ_PIN);
  int voltageMv = map(analogReadVal, 0,4095,0,3300);
  #else
  int voltageMv = ESP.getVcc();
  #endif
  Serial.print(voltageMv);
  Serial.println("mV");
  return voltageMv;
}

void loop() {
}
