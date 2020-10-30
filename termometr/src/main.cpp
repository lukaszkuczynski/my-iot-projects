#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "my_local_wifi.h"



#define SEALEVELPRESSURE_HPA (1013.25)
#define AIO_SERVER        "192.168.0.138"
#define AIO_SERVERPORT    1883     
#define SLEEP_TIME        5e6        
#define LCD_COLUMNS_COUNT 16
#define LCD_ROWS_COUNT    2

Adafruit_BMP085 bme;

float temperature, humidity, pressure, altitude;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Publish temperature_publish = Adafruit_MQTT_Publish(&mqtt, "/esp8622_1/temperature");
Adafruit_MQTT_Publish pressure_publish = Adafruit_MQTT_Publish(&mqtt, "/esp8622_1/pressure");
Adafruit_MQTT_Publish alive_channel = Adafruit_MQTT_Publish(&mqtt, "/esp8622_1/alive");


LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS_COUNT, LCD_ROWS_COUNT);  

void blinkit() {
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

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

void connectWifi() {
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  blinkit();

  Serial.begin(115200);
  if (!bme.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  connectWifi();

  lcd.init();
  lcd.backlight(); 

}

void send_values_serial(float temperature, float pressure) {
  Serial.print(F("Sending values, temperature: ")); 
  Serial.print(temperature);
  Serial.print(", pressure: ");
  Serial.print(pressure);
  Serial.print("\n");
}

void send_values_mqtt(float temperature, float pressure) {
  if (!temperature_publish.publish(temperature)) {
    Serial.println(F("Temperature publishing Failed"));
  }
  if (!pressure_publish.publish(pressure)) {
    Serial.println(F("Pressure publishing Failed"));
  }
}

void display_values_lcd(float temperature, float pressure) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("%.1f C", temperature);
  lcd.setCursor(0,1);
  lcd.printf("%.1f hPa (Luk)", pressure);
}

void loop() {  
  MQTT_connect();
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  send_values_serial(temperature, pressure);
  send_values_mqtt(temperature, pressure);
  display_values_lcd(temperature, pressure);
  delay(1000);
}