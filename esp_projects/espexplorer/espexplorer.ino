//https://madhephaestus.github.io/ESP32Servo/annotated.html
#include <ESP32Servo.h>

#define echoPin 5 // Echo Pin
#define trigPin 4 // Trigger Pin


const int LED_PIN = 2;

const int left_forward_pin = 18;
const int left_backward_pin = 19;
const int right_forward_pin = 25;
const int right_backward_pin = 23;

const int left_forward_channel = 6;
const int left_backward_channel = 7;
const int right_forward_channel = 8;
const int right_backward_channel = 9;

const int freq = 5000;
const int resolution = 8;

const int SERVO_PIN = 26;

const int LEFT_ENCODER_PIN = 34;
const int RIGHT_ENCODER_PIN = 32;

enum LedStates{ON, OFF};

enum VehicleMove {STOP, GO_BACK, GO_FORWARD};
enum Motor {LEFT, RIGHT};
enum Direction {TURN_LEFT, TURN_RIGHT, GO_AHEAD};

LedStates ledState = ON;

VehicleMove vehicleMove = STOP;

int led_delay;
int measure_delay;
 
long duration, distance; // Duration used to calculate distance

unsigned long chrono = millis();

int rightSpeed = 0;
int rightCounter = 0;

int leftSpeed = 0;
int leftCounter = 0;
 
Servo myservo; 

//temporarily we use timer here, in real life encoder will be producing ticks
hw_timer_t * timer = NULL;


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

void IRAM_ATTR onTimer(){
  
}

void stateMachineWalkAlone();

void IRAM_ATTR leftEncoderInterrupt() {
  leftCounter --;
//  stateMachineWalkAlone();
}

void IRAM_ATTR rightEncoderInterrupt() {
  rightCounter --;
//  stateMachineWalkAlone();
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

  // temporarily something that produce ticks
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  pinMode(LEFT_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_PIN, INPUT_PULLUP);
  attachInterrupt(LEFT_ENCODER_PIN, leftEncoderInterrupt, FALLING);
  attachInterrupt(RIGHT_ENCODER_PIN, rightEncoderInterrupt, FALLING);
}

#define STEP_COUNT 7
float distance_map[STEP_COUNT];
#define SERVO_MIN 0
#define SERVO_MAX 180
#define SERVO_STEP (SERVO_MAX-SERVO_MIN) / (STEP_COUNT - 1)

void measure_around() {
  int steps;
  int pos = 0; 
  int step_no = 0;
  for (step_no = 0; step_no <= STEP_COUNT-1; step_no += 1) { 
    pos = (step_no + 1) * SERVO_STEP;
    myservo.write(pos); 
    delay(300);
    distance_map[step_no] = measure_distance();
  }
  myservo.write(90); 

}

  
float measureAhead() {
  int pos = 90; 
  myservo.write(pos); 
  float dist = measure_distance();
  Serial.println(dist);
  return dist;
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
  if (maxDistance < 60) {
    // we are lost, reverse!
    return 270;
  } else {
    return (maxIndex + 1) * SERVO_STEP;
  }
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


void stateMachineWalkAlone() {
  if (rightSpeed > 0) {
    ledcWrite(right_forward_channel, rightSpeed);
    ledcWrite(right_backward_channel, 0);        
  } else {
    ledcWrite(right_backward_channel, abs(rightSpeed));    
    ledcWrite(right_forward_channel, 0);        
  }
  if (leftSpeed > 0) {
    ledcWrite(left_forward_channel, leftSpeed);   
    ledcWrite(left_backward_channel, 0);             
  } else {
    ledcWrite(left_backward_channel, abs(leftSpeed));    
    ledcWrite(left_forward_channel, 0);        
  }
}

void requestTicks(float distanceAhead){
  if (distanceAhead > 50) {
    rightCounter = 50;
    leftCounter = 50;
    rightSpeed = 120;
    leftSpeed = 120;
  } else {
    rightSpeed = 0;
    leftSpeed = 0;
  }
}

void blinkLed() {
 digitalWrite(LED_PIN, HIGH); 
// delay(200);
 digitalWrite(LED_PIN, LOW); 
}

void requestTurnWithAbsolute(int degrees) {
  int localDegrees = 0;
  if (degrees == 90) {
    //no turn, go straight
    requestDrive(GO_AHEAD, 0, 100);
  } else {
    localDegrees = abs(degrees - 90);
    if (degrees < 90) {
      requestDrive(TURN_RIGHT, localDegrees, 0);
    } else {
      requestDrive(TURN_LEFT, localDegrees, 0);
    }
  }
}

void walkAloneWithCounters() {
  stateMachineWalkAlone(); 
  if (rightCounter <= 0 && leftCounter <= 0) {
    stopMotors();
    delay(500);
    float distanceAhead = measureAhead();
    if (distanceAhead > 50) {
      requestDrive(GO_AHEAD, 0, 100);
    } else {
      digitalWrite(LED_PIN, HIGH);
      measure_around();
      int turnCalculated = calculateTurn();
      requestTurnWithAbsolute(turnCalculated);
      digitalWrite(LED_PIN, LOW);
    }
  }  
}

int calculateTicksTurn(int degrees) {
  return degrees / 10;
}

int calculateTicksStraight(int distanceCm) {
  if (distanceCm > 50) {
    return 20;
  } else {
    return 2;
  }
}

void requestDrive(Direction direction, int degrees, int aheadDistance) {
  if (direction == GO_AHEAD) {
    int speed = 120;
    int noOfTicks = calculateTicksStraight(aheadDistance); 
    rightSpeed = speed;
    leftSpeed = speed;
    rightCounter = noOfTicks;
    leftCounter = noOfTicks;
  } else {
    int noOfTicks = calculateTicksTurn(degrees);
    int speed = 120;
    if (direction == TURN_RIGHT) {
      rightSpeed = speed;
      leftSpeed = -1 * speed;
      rightCounter = noOfTicks;
    } else if (direction == TURN_LEFT) {
      leftSpeed = speed;
      rightSpeed = -1 * speed;
      leftCounter = noOfTicks;
    }
  }
}
 
void loop()
{
//  measureAhead();
//  delay(1000);
  walkAloneWithCounters();  
//  avoid_contact_loop();    
}
