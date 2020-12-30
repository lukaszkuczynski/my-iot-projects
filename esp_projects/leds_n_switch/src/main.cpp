#include <Arduino.h>

#define LED_PIN 4
#define SWITCH_PIN 2

ICACHE_RAM_ATTR void detectsClick()
{
  Serial.println("click detected!");
  digitalWrite(LED_PIN, HIGH);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), detectsClick, RISING);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void instant_switch_to_led_loop()
{
  digitalWrite(LED_PIN, digitalRead(SWITCH_PIN));
  delay(100);
}

void three_seconds_delay_led_loop()
{
  digitalWrite(LED_PIN, LOW);
  delay(3000);
}

void loop()
{
  instant_switch_to_led_loop();
  // three_seconds_delay_led_loop();
}
