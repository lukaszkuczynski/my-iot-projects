#include <Arduino.h>

#define LED_PIN 2
bool led_status = false;
String command;
 
void setup() {
    Serial.begin(115200); 
    pinMode(LED_PIN, OUTPUT);
}

void send_led_status(){
  if ( led_status ) {
    Serial.println("LED is ON");
  } else {
    Serial.println("LED is OFF");
  }
} 
void loop() {
    if(Serial.available()){
        command = Serial.readStringUntil('\n');

        Serial.printf("Command received %s \n", command);

        if(command.equals("led=on")){
            digitalWrite(LED_PIN, HIGH);
            led_status = 1;
        } else if(command.equals("led=off")){
            digitalWrite(LED_PIN, LOW);
            led_status = 0;
        } else if(command.equals("ledstatus")){
            send_led_status();
        } else{
            Serial.println("Invalid command");
        }
    }
}
