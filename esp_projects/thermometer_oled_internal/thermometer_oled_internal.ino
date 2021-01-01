#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP085 bmp;

void setup() {
  
  Serial.begin(9600);
  Serial.println("Starting");

  Wire.begin();

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
}

void display_oled_temp(float temp) {
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner  
  display.setTextSize(3);      
  display.setTextColor(SSD1306_WHITE); 
  display.print(temp);
  display.println("C");
  display.display();  
}


void loop() {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
   
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  display_oled_temp(bmp.readTemperature());
  
  delay(1000);

}
