
void setup() {
 Serial.begin (9600);
 pinMode (4, INPUT); //Sensor output
}
void loop() {
Serial.print ("Sensor: ");
 Serial.println (digitalRead(4)); //print the sensor output
 delay (500); //wait half a second
}
