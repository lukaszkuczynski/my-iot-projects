#define echoPin 5 // Echo Pin
#define trigPin 4 // Trigger Pin
const int LED_PIN = 2;

enum LedStates{ON, OFF};
LedStates ledState = ON;

int led_delay;
 
long duration, distance; // Duration used to calculate distance

unsigned long chrono = millis();
 
void setup()
{
  led_delay = 1000;
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

float measure_distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.println(distance);
  return distance;  
}

// https://github.com/j-bellavance/Tutorials/blob/master/State%20machines%20Tutorial/Part%201/BlinkWithoutDelay/BlinkWithoutDelay.ino
void blinkMachine() {
  switch (ledState) {
    case OFF: {
      if (millis() - chrono >= led_delay) {
        chrono = millis();
        digitalWrite(LED_PIN, HIGH);
        ledState = ON;
      }
      break;
    }
    case ON: { 
      if (millis() - chrono >= led_delay) {
        chrono = millis();
        digitalWrite(LED_PIN, LOW);
        ledState = OFF;
      }
      break;
    }
  }
}

 
void loop()
{
  float distance = measure_distance();  
  led_delay = 10 * distance;
  blinkMachine();
  delay(50);
}
