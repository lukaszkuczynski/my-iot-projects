hw_timer_t * timer = NULL;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
int state = 0;

void IRAM_ATTR onTimer(){
  isrCounter++;
  lastIsrAt = millis();
}

void setup() {
  // put your setup code here, to run once:
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 1000, true);

  // Start an alarm
  timerAlarmEnable(timer);
  pinMode (2, OUTPUT);
  
}

void loop() {
  if (isrCounter > 1000) {
    isrCounter = 0;
    state = state >> 1;
    digitalWrite(2, !digitalRead(2));  
  }
  delay(20);
}
