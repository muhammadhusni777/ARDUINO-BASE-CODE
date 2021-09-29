
#include <DigiPotX9Cxxx.h>

DigiPot pot(2,3,4);
int pot_val;
int vc_pin_1 = 10;
int vc_pin_2 = 9;
float voltage;
int sp_buff;
float sp;
float adc;
float kp = 20;
int p_control;
float error;
void setup() {
  // put your setup code here, to run once:
pinMode(vc_pin_1, OUTPUT);
pinMode(vc_pin_2, OUTPUT);
Serial.begin(9600);
pot.increase(50);

}

void loop() {
  // put your main code here, to run repeatedly:
 
  if (Serial.available()){
  sp_buff = Serial.parseInt();
  }

  if (sp_buff > 0){
    Serial.println("a");
    digitalWrite(vc_pin_1, HIGH);
    digitalWrite(vc_pin_2, LOW);
  } else {
    Serial.println("b");
    digitalWrite(vc_pin_1, LOW);
    digitalWrite(vc_pin_2, HIGH);
  }


  sp = abs(float(sp_buff)/10);
  adc = analogRead(A0);  
  voltage = (adc*5/1023)*(6.7/2.35);
  
  error = sp - voltage;
  p_control = kp*error;
  pot_val = p_control;
  if (p_control > 0){
   pot.decrease(abs(pot_val)); 
  } 
  if (p_control < 0) {
   pot.increase(abs(pot_val));
  }
  
  Serial.print(sp);
  Serial.print(" ");
  Serial.print(voltage);
  Serial.print(" ");
  Serial.println(voltage);
}
