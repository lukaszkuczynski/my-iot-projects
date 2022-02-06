#include "mqtt_communication.h"

const int pirMotionPin = 5; // D1
const int relayPin = 4; // D2
const int ledPin = 16; // D0




void resetRelay() {
  Serial.println("relay reset");
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
}


boolean detected = false;

ICACHE_RAM_ATTR void detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  detected = true;  
}

void setup() {
  pinMode(pirMotionPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println("Started");
  resetRelay();  // PIR Motion Sensor mode INPUT_PULLUP

  setup_wifi();
  
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(pirMotionPin), detectsMovement, RISING);

  
}

void loop() 
{
  if (detected) {
    detected = false;
    resetRelay();
    sendState("pir_01", "move");
  }  
}
