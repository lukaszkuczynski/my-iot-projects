//https://madhephaestus.github.io/ESP32Servo/annotated.html
#include <ESP32Servo.h>

#define echoPin 5 // Echo Pin
#define trigPin 4 // Trigger Pin


const int LED_PIN = 2;

const int left_forward_pin = 18;
const int left_backward_pin = 19;
const int right_forward_pin = 22;
const int right_backward_pin = 23;

const int left_forward_channel = 6;
const int left_backward_channel = 7;
const int right_forward_channel = 8;
const int right_backward_channel = 9;

const int freq = 5000;
const int resolution = 8;

const int SERVO_PIN = 21;

enum LedStates{ON, OFF};

enum VehicleMove {STOP, GO_BACK, GO_FORWARD};

LedStates ledState = ON;

VehicleMove vehicleMove = STOP;

int led_delay;
int measure_delay;
 
long duration, distance; // Duration used to calculate distance

unsigned long chrono = millis();


 
Servo myservo; 

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

  ledcAttachPin(SERVO_PIN, 4);

}
 
void setup()
{
  led_delay = 1000;
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  setupMotors();
  myservo.setPeriodHertz(50);    
  myservo.attach(SERVO_PIN, 500, 2400); 
}

#define STEP_COUNT 5
float distance_map[STEP_COUNT];
#define SERVO_MIN 30
#define SERVO_MAX 150
#define SERVO_STEP (SERVO_MAX-SERVO_MIN) / (STEP_COUNT - 1)

void measure_around() {
  int steps;
  int pos = 0; 
  int step_no = 0;
  for (step_no = 0; step_no <= STEP_COUNT-1; step_no += 1) { 
    pos = (step_no + 1) * SERVO_STEP;
    myservo.write(pos); 
    delay(1000);
    distance_map[step_no] = measure_distance();
  }
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

void turnVehicleToDegrees(int to_degrees) {
  int dutyCycle = 100;
  int delay60 = 250;
  int delay30 = delay60 / 2;

  Serial.println("Turning..");
  Serial.println(to_degrees);
  switch (to_degrees) {
    case 30:
      ledcWrite(left_forward_channel, dutyCycle);
      ledcWrite(right_backward_channel, dutyCycle);
      delay(delay60);
      break;
    case 60:
      ledcWrite(left_forward_channel, dutyCycle);
      ledcWrite(right_backward_channel, dutyCycle);
      delay(delay30);
      break;
    case 90:
      break;
    case 120:
      ledcWrite(right_forward_channel, dutyCycle);
      ledcWrite(left_backward_channel, dutyCycle);
      delay(delay30);
      break;
    case 150:
      ledcWrite(right_forward_channel, dutyCycle);
      ledcWrite(left_backward_channel, dutyCycle);
      delay(delay60);
      break;
  }
  delay(500);
}

void motorsGo(VehicleMove vehicleMove, int dutyCycle) {
  switch (vehicleMove) {
    case GO_BACK:
      ledcWrite(left_forward_channel, 0);
      ledcWrite(right_forward_channel, 0);
      ledcWrite(left_backward_channel, dutyCycle);
      ledcWrite(right_backward_channel, dutyCycle);
      break;
    case GO_FORWARD:
      ledcWrite(left_forward_channel, dutyCycle);
      ledcWrite(right_forward_channel, dutyCycle);
      ledcWrite(left_backward_channel, 0);
      ledcWrite(right_backward_channel, 0);
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


void avoid_contact_loop() {
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

int calculateTurn() {
  for (int i = 0; i < STEP_COUNT ; i++) {Serial.print(distance_map[i]); Serial.print(" ");}
  Serial.println("--");
  float maxDistance = 0;
  int maxIndex = -1;
  for (int i = 0; i < STEP_COUNT ; i++) {
    if (distance_map[i] > maxDistance) {
      maxDistance = distance_map[i];
      maxIndex = i;
    }
  }
  Serial.print("Max index "); Serial.print(maxIndex); Serial.print(", max distance"); Serial.println(maxDistance);
  return (maxIndex + 1) * SERVO_STEP;
}

void walk_alone_loop() {
    measure_around();
    int turnCalculated = calculateTurn();
    turnVehicleToDegrees(turnCalculated);
    stopMotors();    
    motorsGo(GO_FORWARD, 150);
    delay(2000);
    stopMotors();
}
 
void loop()
{
  walk_alone_loop();
}
