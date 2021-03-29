#define echoPin 5 // Echo Pin
#define trigPin 2 // Trigger Pin
const int LED_PIN = 4;
 
long duration, distance; // Duration used to calculate distance
 
void setup()
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
}
 
void loop()
{
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.println(distance);
  //Delay 50ms before next reading.
  digitalWrite(LED_PIN, LOW);
  delay(500);

  
}
