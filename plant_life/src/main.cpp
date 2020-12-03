#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  pinMode(D0, WAKEUP_PULLUP);
  pinMode(D7, OUTPUT);
}

int loopval = 0;

void loop()
{

  digitalWrite(D7, HIGH);
  delay(500);

  int sensorValue = analogRead(A0);

  // map it to the range of the PWM out
  int outputValue = map(sensorValue, 0, 1023, 0, 3300);

  // print the readings in the Serial Monitor
  Serial.printf("%d. %d\n", loopval, outputValue);
  loopval++;
  digitalWrite(D7, LOW);
  delay(1000);

  // ESP.deepSleep(5e6);
}