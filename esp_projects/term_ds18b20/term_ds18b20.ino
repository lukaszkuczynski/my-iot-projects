#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 5;     

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);  



void setup() {
  Serial.begin(115200);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");  
  delay(1000);
}
