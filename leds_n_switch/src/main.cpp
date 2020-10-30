#include <Arduino.h>

int ledPin = 12; 

int SWITCH_PIN = 14;
 

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // ustawiamy pin jako wyjście
  pinMode(SWITCH_PIN, INPUT);
}
 
// funkcja loop uruchamia się w nieskończonej pętli
void loop() {
  digitalWrite(ledPin, LOW);   // włączmy diodę, podajemy stan wysoki

  int read = digitalRead(SWITCH_PIN);
  

  if (read == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  
}