const int PIR_SENSOR = 2;


void setup() {
  Serial.begin(115200);
  pinMode(PIR_SENSOR, INPUT);
}

int val = 0;

void loop() {
  val = digitalRead(PIR_SENSOR);
  Serial.println(val);
  delay(1000);
}
