const int REED_SENSOR = 2;
const int BUZZER = 5  ;


void setup() {
  Serial.begin(115200);
  pinMode(REED_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);
}

int val;

void wait_before_beep(int cycles) {
  for (int i=cycles; i > 0; i--) {
    Serial.print("Cycles to go: ");Serial.println(i);
    delay(1000);
  }
  val = digitalRead(REED_SENSOR);
  digitalWrite(BUZZER, val);
  delay(1000);
}

void loop() {
  val = digitalRead(REED_SENSOR);
  if (val == 1) {
    wait_before_beep(3);
  }
  Serial.println(val);
  digitalWrite(BUZZER, val);
  delay(100);
}
