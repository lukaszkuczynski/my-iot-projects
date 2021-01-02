#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "secrets.h" // this file has definitions for 1)WIFI_SSID, 2) WIFI_PASSWORD, 3)MQTT_SERVER url

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP085 bmp;
 
ADC_MODE(ADC_VDD); // measure internal voltage, please

const int OLED_TOGGLE_PIN = 14;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, 1883);
Adafruit_MQTT_Publish temperature_publish = Adafruit_MQTT_Publish(&mqtt, "lukmqtt/internal_1/temperature");
Adafruit_MQTT_Publish voltage_publish = Adafruit_MQTT_Publish(&mqtt, "lukmqtt/internal_1/voltage");

const int SLEEP_TIME = 1e6 * 60 * 5;

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(" WiFi connected."));
}

void sendTelemetry(float temp, int voltage) {
  MQTT_connect();
  int ret;
  ret = temperature_publish.publish(temp);
  if (!ret) {
    Serial.println("Failed sending telemetry!!");
  } else {
    Serial.println("Telemetry sent.");
  }
  ret = voltage_publish.publish(voltage);
  if (!ret) {
    Serial.println("Failed sending telemetry!!");
  } else {
    Serial.println("Telemetry sent.");
  }

}

void setup() {
  
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(OLED_TOGGLE_PIN, OUTPUT);

  Wire.begin();

  digitalWrite(OLED_TOGGLE_PIN, HIGH); // Turn on the display
  delay(500);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text  
  display.println("Starting app..");
  display.println("Thermometer, by Luke");
  display.display();

  if (!bmp.begin()) { 
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  float temperature = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
  int internalVoltage = readInternalVoltage();

  displayTelemetryOLED(temperature, internalVoltage);

  connectWifi();
  sendTelemetry(temperature, internalVoltage);
  
  delay(5000);
  digitalWrite(OLED_TOGGLE_PIN, LOW); // Turn off the display
  Serial.println("Going sleep, bye");

  ESP.deepSleep(SLEEP_TIME); 
}

void displayTelemetryOLED(float temp, int voltage) {
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner  
  display.setTextSize(3);      
  display.setTextColor(SSD1306_WHITE); 
  display.print(temp);
  display.println("C");
  // display battery state
  display.setTextSize(1);
  display.print("battery: ");
  display.print(voltage);
  display.print("mV");
  display.display();  
}

int readInternalVoltage() {
  int internalVoltage = ESP.getVcc();
  Serial.printf("ADC %f\n", internalVoltage);
  return internalVoltage;
}

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void loop() {
}
