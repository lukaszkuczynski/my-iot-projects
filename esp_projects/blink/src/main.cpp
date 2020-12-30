#include <Arduino.h>

const int LED_ESP32 = 2;

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  pinMode(LED_ESP32, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
  Serial.println("z");
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_ESP32, HIGH);
  delay(1000);                    // wait for a second
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  digitalWrite(LED_ESP32, LOW);
  delay(1000); // wait for a second
}
