const int left_forward_pin = 18;  
const int left_backward_pin = 19;
const int right_forward_pin = 22;  
const int right_backward_pin = 23;

const int left_forward_channel = 0;
const int left_backward_channel = 1;
const int right_forward_channel = 2;
const int right_backward_channel = 3;


const int freq = 50;
const int resolution = 8;
 
void setup(){
//  pinMode(left_forward, OUTPUT);
//  pinMode(left_backward, OUTPUT);
  Serial.begin(115200);
  ledcSetup(left_forward_channel, freq, resolution);
  ledcSetup(left_backward_channel, freq, resolution);
  ledcSetup(right_forward_channel, freq, resolution);
  ledcSetup(right_backward_channel, freq, resolution);
  
  ledcAttachPin(left_forward_pin, left_forward_channel);
  ledcAttachPin(left_backward_pin, left_backward_channel);
  ledcAttachPin(right_forward_pin, right_forward_channel);
  ledcAttachPin(right_backward_pin, right_backward_channel);
}
 
void loop(){
  Serial.println("forward!");
  ledcWrite(left_backward_channel, 0);
  ledcWrite(right_backward_channel, 0);
   
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(left_forward_channel, dutyCycle);
    ledcWrite(right_forward_channel, dutyCycle);
    delay(15);
  }

  delay(1000);

  Serial.println("back..");
  ledcWrite(left_forward_channel, 0);
  ledcWrite(right_forward_channel, 0);
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(left_backward_channel, dutyCycle);
    ledcWrite(right_backward_channel, dutyCycle);
    delay(15);
  }

  delay(1000);
}
