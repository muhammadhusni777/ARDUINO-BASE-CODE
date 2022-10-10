int motor_forward = 7;
int motor_reverse = 8;

int pot_adc;
int motor_degree;


String status_motor;
char in_message;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motor_forward, OUTPUT);
  pinMode(motor_reverse, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    in_message = Serial.read();
    if (in_message == 'F'){
      digitalWrite(motor_forward, HIGH);
      digitalWrite(motor_reverse, LOW);
      status_motor = "forward";
    }
    if (in_message == 'S'){
      digitalWrite(motor_forward, LOW);
      digitalWrite(motor_reverse, LOW);
      status_motor = "stop";
    }
    if (in_message == 'R'){
      digitalWrite(motor_forward, LOW);
      digitalWrite(motor_reverse, HIGH);
      status_motor = "reverse";
    }
  }
  
  Serial.print(status_motor);
  Serial.print("  ");
  pot_adc = analogRead(A0);
  motor_degree = map(pot_adc, 0, 1023, 0, 360);
  Serial.print(motor_degree);
}
