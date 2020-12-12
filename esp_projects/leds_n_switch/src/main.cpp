#include <Arduino.h>

#define LED_PIN 12
#define SWITCH_PIN 14
 
ICACHE_RAM_ATTR void detectsClick() {
  Serial.println("click detected!");
  digitalWrite(LED_PIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT); 
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), detectsClick, RISING);
}

void instant_switch_to_led_loop() {
  digitalWrite(LED_PIN, digitalRead(SWITCH_PIN));
  delay(100);
}

void three_seconds_delay_led_loop() {
  digitalWrite(LED_PIN, LOW);   
  delay(3000);
} 

void loop() {
  instant_switch_to_led_loop();
  // three_seconds_delay_led_loop();
}

