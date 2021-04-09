//https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/
struct Encoder {
  const uint8_t PIN;
  uint32_t ticks;
  bool pressed;
};

Encoder button1 = {34, 0, false};

void IRAM_ATTR isr() {
  button1.ticks += 1;
  button1.pressed = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

void loop() {
  if (button1.pressed) {
      Serial.printf("Encoder seen %u holes\n", button1.ticks);
      button1.pressed = false;
  }

  //Detach Interrupt after 1 Minute
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);
  Serial.println("Interrupt Detached!");
  }
}
