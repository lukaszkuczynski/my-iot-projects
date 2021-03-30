#define echoPin 5 // Echo Pin
#define trigPin 4 // Trigger Pin

const int LED_PIN = 2;

const int left_forward_pin = 18;
const int left_backward_pin = 19;
const int right_forward_pin = 22;
const int right_backward_pin = 23;

const int left_forward_channel = 0;
const int left_backward_channel = 1;
const int right_forward_channel = 2;
const int right_backward_channel = 3;

const int freq = 5000;
const int resolution = 8;

enum LedStates{ON, OFF};

enum VehicleMove {STOP, GO_BACK};

LedStates ledState = ON;

VehicleMove vehicleMove = STOP;

int led_delay;
 
long duration, distance; // Duration used to calculate distance

unsigned long chrono = millis();

void setupMotors()
{
  ledcSetup(left_forward_channel, freq, resolution);
  ledcSetup(left_backward_channel, freq, resolution);
  ledcSetup(right_forward_channel, freq, resolution);
  ledcSetup(right_backward_channel, freq, resolution);

  ledcAttachPin(left_forward_pin, left_forward_channel);
  ledcAttachPin(left_backward_pin, left_backward_channel);
  ledcAttachPin(right_forward_pin, right_forward_channel);
  ledcAttachPin(right_backward_pin, right_backward_channel);
}
 
void setup()
{
  led_delay = 1000;
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  setupMotors();
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

void stopMotors() {
  ledcWrite(left_backward_channel, 0);
  ledcWrite(right_backward_channel, 0);
  ledcWrite(left_forward_channel, 0);
  ledcWrite(right_forward_channel, 0);  
}

void motorsGo(VehicleMove vehicleMove, int dutyCycle) {
  switch (vehicleMove) {
    case GO_BACK:
      ledcWrite(left_forward_channel, 0);
      ledcWrite(right_forward_channel, 0);
      ledcWrite(left_backward_channel, dutyCycle);
      ledcWrite(right_backward_channel, dutyCycle);
      break;
  }
}

void motorStateMachine() {
  switch (vehicleMove) {
    case STOP:
      stopMotors();
      break;
    case GO_BACK:
      motorsGo(GO_BACK, 150);
      break;
  }
}

 
void loop()
{
  float distance = measure_distance();  
  led_delay = 10 * distance;
  blinkMachine();
  if (distance < 50) {
    vehicleMove = GO_BACK;
  } else {
    vehicleMove = STOP;
  }
  motorStateMachine();
  delay(50);
}
