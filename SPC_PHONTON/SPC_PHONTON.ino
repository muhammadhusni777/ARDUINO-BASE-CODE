
/*
mosquitto_pub -h 123.45.0.10 -p 1883 -t hoho -m *sudut servo*


*/
#include <TinyGPS++.h>
// Create a TinyGPS++ object
TinyGPSPlus gps;




#include <Wire.h>
//#include <MechaQMC5883.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <avr/wdt.h>

//#include <LwIP.h>
//#include <STM32Ethernet.h>

//#include<IWatchdog.h>

String control_mode = "open_loop";
String steering_state1;
String steering_state2;
String steering_state3;
String steering_state4;

String speed_state1;
String speed_state2;
String speed_state3;
String speed_state4;

unsigned long time_now;
unsigned long time_elapsed;
unsigned long time_prev;

unsigned long time_send;
unsigned long time_send_prev;
int time_message = 1000;

//MechaQMC5883 qmc;
int azimuth =0;
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xE6 }; //0x00, 0x80, 0xE1, 0x01, 0x01, 0x01
//byte mac[]    = {  0x00, 0x80, 0xE1, 0x01, 0x01, 0x01 };

IPAddress ip(123, 45, 0, 106);
IPAddress server(123, 45, 0, 10);

float latitude;
float longitude;

int rpm_pulse1;
int rpm_pulse2;
int rpm_pulse3;
int rpm_pulse4;

int rpm_propeller1;
int rpm_propeller2;
int rpm_propeller3;
int rpm_propeller4;


int propeller1_speed_pos;
int propeller1_speed_neg;
int propeller1_speed_sp;
int propeller1_speed_e;
int propeller1_steering_pos = 26;
int propeller1_steering_neg = 28;
int propeller1_steering_deg;
int propeller1_steering_sp;
int propeller1_steering_e;
float kp_speed1;
float p_control_speed1;
float ki_speed1;
float i_control_speed1;
float kd_speed1;
float d_control_speed1;
float pid_control_speed1;


int propeller2_speed_pos;
int propeller2_speed_neg;
int propeller2_speed_sp;
int propeller2_speed_e;
int propeller2_steering_pos = 32;
int propeller2_steering_neg = 34;
int propeller2_steering_deg;
int propeller2_steering_sp;
int propeller2_steering_e;
float kp_speed2;
float p_control_speed2;
float ki_speed2;
float i_control_speed2;
float kd_speed2;
float d_control_speed2;
float pid_control_speed2;


int propeller3_speed_pos;
int propeller3_speed_neg;
int propeller3_speed_sp;
int propeller3_speed_e;
int propeller3_steering_pos;
int propeller3_steering_neg;
int propeller3_steering_deg;
int propeller3_steering_sp;
int propeller3_steering_e;
float kp_speed3;
float p_control_speed3;
float ki_speed3;
float i_control_speed3;
float kd_speed3;
float d_control_speed3;
float pid_control_speed3;


int propeller4_speed_pos;
int propeller4_speed_neg;
int propeller4_speed_sp;
int propeller4_speed_e;
int propeller4_steering_pos;
int propeller4_steering_neg;
int propeller4_steering_deg;
int propeller4_steering_sp;
int propeller4_steering_e;
float kp_speed4;
float p_control_speed4;
float ki_speed4;
float i_control_speed4;
float kd_speed4;
float d_control_speed4;
float pid_control_speed4;




void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageTemp += (char)message[i];
    
  }
  //val = messageTemp.toFloat();
 
  //Serial.println();
   if (String(topic) == "test_topic") {
    Serial.print(messageTemp);
   }

   if (String(topic) == "control_mode") {
    Serial.print(messageTemp);
    control_mode = messageTemp;
   }
   if (String(topic) == "steering_state1") {
    Serial.print(messageTemp);
    steering_state1 = messageTemp;
   }

   if (String(topic) == "steering_state2") {
    Serial.print(messageTemp);
    steering_state2 = messageTemp;
   }
  if (String(topic) == "steering_state3") {
    Serial.print(messageTemp);
    steering_state3 = messageTemp;
   }
   if (String(topic) == "steering_state4") {
    Serial.print(messageTemp);
    steering_state4 = messageTemp;
   }

   
   
  if (String(topic) == "hoho") {
    Serial.print("Changing output to ");
     
     if(messageTemp == "4on"){    
      Serial.println("off");  
      digitalWrite(4, HIGH);
    }
     else if(messageTemp == "4off"){
      Serial.println("off");
      digitalWrite(4, LOW);
    }
  }

 messageTemp ="";
}

EthernetClient ethClient;
PubSubClient client(ethClient);
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("CompassClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("hoho");
      client.subscribe("test_topic");
      
      client.subscribe("speedinput1");
      client.subscribe("speedinput2");
      client.subscribe("speedinput3");
      client.subscribe("speedinput4");

      client.subscribe("steering1");
      client.subscribe("steering2");
      client.subscribe("steering3");
      client.subscribe("steering4");
      client.subscribe("control_mode");
      client.subscribe("steering_state1");
      client.subscribe("steering_state2");
      client.subscribe("steering_state3");
      client.subscribe("steering_state4");

      client.subscribe("speed_state1");
      client.subscribe("speed_state2");
      client.subscribe("speed_state3");
      client.subscribe("speed_state4");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  } 
}





void setup() {

  pinMode(propeller1_speed_pos, OUTPUT);
  pinMode(propeller2_speed_pos, OUTPUT);
  pinMode(propeller3_speed_pos, OUTPUT);
  pinMode(propeller4_speed_pos, OUTPUT);
  pinMode(propeller1_speed_neg, OUTPUT);
  pinMode(propeller2_speed_neg, OUTPUT);
  pinMode(propeller3_speed_neg, OUTPUT);
  pinMode(propeller4_speed_neg, OUTPUT);

  
  pinMode(propeller1_steering_pos, OUTPUT);
  pinMode(propeller2_steering_pos, OUTPUT);
  pinMode(propeller3_steering_pos, OUTPUT);
  pinMode(propeller4_steering_pos, OUTPUT);
  pinMode(propeller1_steering_neg, OUTPUT);
  pinMode(propeller2_steering_neg, OUTPUT);
  pinMode(propeller3_steering_neg, OUTPUT);
  pinMode(propeller4_steering_neg, OUTPUT);

  Wire.begin();
  //Serial2.begin(9600);
  Serial.begin(115200);
  Serial.println("booting....");
  //qmc.init();
  client.setServer(server, 1883);
  client.setCallback(callback);
  pinMode (4, OUTPUT);
  Ethernet.init(10);  
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  attachInterrupt(2, rpm_tick1, RISING);
  attachInterrupt(3, rpm_tick2, RISING);
  attachInterrupt(18, rpm_tick3, RISING);
  attachInterrupt(19, rpm_tick4, RISING);

  
  
  //delay(1500);
  wdt_enable(WDTO_8S);
}

void rpm_tick1(){
  rpm_pulse1++;
}

void rpm_tick2(){
  rpm_pulse2++;
}

void rpm_tick3(){
  rpm_pulse3++;
}

void rpm_tick4(){
  rpm_pulse4++;
}

static char yaw_send[15];
static char rpm_propeller1_send[15];
static char rpm_propeller2_send[15];
static char rpm_propeller3_send[15];
static char rpm_propeller4_send[15];
static char time_elapsed_send[15];

static char propeller1_steering_deg_send[15];
static char propeller2_steering_deg_send[15];
static char propeller3_steering_deg_send[15];
static char propeller4_steering_deg_send[15];

void loop() {
  time_now = millis();
  
  if (!client.connected()) {
    reconnect();
  }
///////////////////////PROPELLER CONTROL///////////////////////////////////////////////////
  Serial.println(control_mode);
  if (control_mode == "close_loop"){
  propeller_control_1_close_loop();
  propeller_control_2_close_loop();
  propeller_control_3_close_loop();
  propeller_control_4_close_loop();
  }

   if (control_mode == "open_loop"){
  propeller_control_1_open_loop();
  propeller_control_2_open_loop();
  propeller_control_3_open_loop();
  propeller_control_4_open_loop();
  
  } 
  
    
  




///////////////////////////send data and rpm calculation///////////////////////////////////
  time_send = millis() - time_send_prev;
  if (time_send > time_message){
  rpm_propeller1 = rpm_pulse1*60 / time_send;
  rpm_propeller2 = rpm_pulse2*60 / time_send;
  rpm_propeller3 = rpm_pulse3*60 / time_send;
  rpm_propeller4 = rpm_pulse4*60 / time_send;
  
  
 
  client.publish("Yaw",dtostrf(azimuth,6,3,yaw_send)); // kirim data
  client.publish("mesin1",dtostrf(rpm_propeller1,6,3,rpm_propeller1_send));
  client.publish("mesin2",dtostrf(rpm_propeller2,6,3,rpm_propeller2_send));
  client.publish("mesin3",dtostrf(rpm_propeller3,6,3,rpm_propeller3_send));
  client.publish("mesin4",dtostrf(rpm_propeller4,6,3,rpm_propeller4_send));
  client.publish("tperloop",dtostrf(time_elapsed,6,5,time_elapsed_send));

  client.publish("azimuth1",dtostrf(propeller1_steering_deg,6,3,propeller1_steering_deg_send));
  client.publish("azimuth2",dtostrf(propeller2_steering_deg,6,3,propeller2_steering_deg_send));
  client.publish("azimuth3",dtostrf(propeller3_steering_deg,6,3,propeller3_steering_deg_send));
  client.publish("azimuth4",dtostrf(propeller4_steering_deg,6,3,propeller4_steering_deg_send));
  
  time_send_prev = millis();
  
  rpm_pulse1 = 0;
  rpm_pulse2 = 0;
  rpm_pulse3 = 0;
  rpm_pulse4 = 0;
  }
  
  
  client.loop();
 // delay(500);
  time_elapsed = time_now - time_prev;
  time_prev = time_now;

  wdt_reset();
}


void propeller_control_1_close_loop(){
  //steering
  //Serial.println("fail");
  propeller1_steering_deg = map(analogRead(A0),0,1023,0,360);
  propeller1_steering_e = propeller1_steering_sp - propeller1_steering_deg;

  //on off control
  if (propeller1_steering_e < -5){
  digitalWrite(propeller1_steering_pos, LOW);
  digitalWrite(propeller1_steering_neg, HIGH);
  } 
  
  else if (propeller1_steering_e > 5){
  digitalWrite(propeller1_steering_pos, HIGH);
  digitalWrite(propeller1_steering_neg, LOW);
  }

  else {
  digitalWrite(propeller1_steering_pos, LOW);
  digitalWrite(propeller1_steering_neg, LOW);
  }
  
  
  
  //speed

  propeller1_speed_e = propeller1_speed_sp - rpm_propeller1;
  
}

void propeller_control_1_open_loop(){
  Serial.print(" dt :");
  Serial.println(steering_state1);
  //steering
  propeller1_steering_deg = map(analogRead(A0),0,1023,0,360);
  propeller1_steering_e = propeller1_steering_sp - propeller1_steering_deg;

  //manual control
  if (steering_state1 == "left"){
  digitalWrite(propeller1_steering_pos, LOW);
  digitalWrite(propeller1_steering_neg, HIGH);
  
  } 
  
  else if (steering_state1 == "right"){
  digitalWrite(propeller1_steering_pos, HIGH);
  digitalWrite(propeller1_steering_neg, LOW);
  }

  else if(steering_state1 == "stop") {
  digitalWrite(propeller1_steering_pos, LOW);
  digitalWrite(propeller1_steering_neg, LOW);
  }
  
  
  
  //speed

  propeller1_speed_e = propeller1_speed_sp - rpm_propeller1;

  if (speed_state1 == "increase"){
  digitalWrite(propeller1_speed_pos, LOW);
  digitalWrite(propeller1_speed_neg, HIGH);
  } 
  
  if (steering_state1 == "decrease"){
  digitalWrite(propeller1_speed_pos, HIGH);
  digitalWrite(propeller1_speed_neg, LOW);
  }

  if(steering_state1 == "stop") {
  digitalWrite(propeller1_speed_pos, LOW);
  digitalWrite(propeller1_speed_neg, LOW);
  }
  
}



void propeller_control_2_close_loop(){
  //steering
  propeller2_steering_deg = map(analogRead(A1),0,1023,0,360);
  propeller2_steering_e = propeller2_steering_sp - propeller2_steering_deg;

  //on off control
  if (propeller2_steering_e < -5){
  digitalWrite(propeller2_steering_pos, LOW);
  digitalWrite(propeller2_steering_neg, HIGH);
  } 
  
  else if (propeller2_steering_e > 5){
  digitalWrite(propeller2_steering_pos, HIGH);
  digitalWrite(propeller2_steering_neg, LOW);
  }

  else {
  digitalWrite(propeller2_steering_pos, LOW);
  digitalWrite(propeller2_steering_neg, LOW);
  }
  
  //speed
   propeller2_speed_e = propeller2_speed_sp - rpm_propeller2;
}


void propeller_control_2_open_loop(){
  //steering
  propeller2_steering_deg = map(analogRead(A0),0,1023,0,360);
  propeller2_steering_e = propeller2_steering_sp - propeller2_steering_deg;

  //manual control
  if (steering_state2 == "left"){
  digitalWrite(propeller2_steering_pos, LOW);
  digitalWrite(propeller2_steering_neg, HIGH);
  } 
  
  if (steering_state2 == "right"){
  digitalWrite(propeller2_steering_pos, HIGH);
  digitalWrite(propeller2_steering_neg, LOW);
  }

  if(steering_state2 == "stop") {
  digitalWrite(propeller2_steering_pos, LOW);
  digitalWrite(propeller2_steering_neg, LOW);
  }
  
  
  
  //speed

  propeller2_speed_e = propeller2_speed_sp - rpm_propeller2;

  if (speed_state2 == "increase"){
  digitalWrite(propeller2_speed_pos, LOW);
  digitalWrite(propeller2_speed_neg, HIGH);
  } 
  
  if (steering_state2 == "right"){
  digitalWrite(propeller2_speed_pos, HIGH);
  digitalWrite(propeller2_speed_neg, LOW);
  }

  if(steering_state2 == "stop") {
  digitalWrite(propeller2_speed_pos, LOW);
  digitalWrite(propeller2_speed_neg, LOW);
  }
  
}



void propeller_control_3_close_loop(){
  //steering
  propeller3_steering_deg = map(analogRead(A2),0,1023,0,360);
  propeller3_steering_e = propeller3_steering_sp - propeller3_steering_deg;

  //on off control
  if (propeller3_steering_e < -5){
  digitalWrite(propeller3_steering_pos, LOW);
  digitalWrite(propeller3_steering_neg, HIGH);
  } 
  
  else if (propeller3_steering_e > 5){
  digitalWrite(propeller3_steering_pos, HIGH);
  digitalWrite(propeller3_steering_neg, LOW);
  }

  else {
  digitalWrite(propeller3_steering_pos, LOW);
  digitalWrite(propeller3_steering_neg, LOW);
  }

  //speed
   propeller3_speed_e = propeller3_speed_sp - rpm_propeller3;
}

void propeller_control_3_open_loop(){
  //steering
  propeller3_steering_deg = map(analogRead(A0),0,1023,0,360);
  propeller3_steering_e = propeller3_steering_sp - propeller3_steering_deg;

  //manual control
  if (steering_state3 == "left"){
  digitalWrite(propeller3_steering_pos, LOW);
  digitalWrite(propeller3_steering_neg, HIGH);
  } 
  
  if (steering_state3 == "right"){
  digitalWrite(propeller3_steering_pos, HIGH);
  digitalWrite(propeller3_steering_neg, LOW);
  }

  if(steering_state3 == "stop") {
  digitalWrite(propeller3_steering_pos, LOW);
  digitalWrite(propeller3_steering_neg, LOW);
  }
  
  
  
  //speed

  propeller3_speed_e = propeller3_speed_sp - rpm_propeller3;

  if (speed_state3 == "increase"){
  digitalWrite(propeller3_speed_pos, LOW);
  digitalWrite(propeller3_speed_neg, HIGH);
  } 
  
  if (steering_state3 == "right"){
  digitalWrite(propeller3_speed_pos, HIGH);
  digitalWrite(propeller3_speed_neg, LOW);
  }

  if(steering_state3 == "stop") {
  digitalWrite(propeller3_speed_pos, LOW);
  digitalWrite(propeller3_speed_neg, LOW);
  }
  
}



void propeller_control_4_close_loop(){
  //steering
  propeller4_steering_deg = map(analogRead(A3),0,1023,0,360);
  propeller4_steering_e = propeller4_steering_sp - propeller4_steering_deg;

  //on off control
  if (propeller4_steering_e < -5){
  digitalWrite(propeller4_steering_pos, LOW);
  digitalWrite(propeller4_steering_neg, HIGH);
  } 
  
  else if (propeller4_steering_e > 5){
  digitalWrite(propeller4_steering_pos, HIGH);
  digitalWrite(propeller4_steering_neg, LOW);
  }

  else {
  digitalWrite(propeller4_steering_pos, LOW);
  digitalWrite(propeller4_steering_neg, LOW);
  }

  //speed
   propeller4_speed_e = propeller4_speed_sp - rpm_propeller4;
}

void propeller_control_4_open_loop(){
  //steering
  propeller4_steering_deg = map(analogRead(A0),0,1023,0,360);
  propeller4_steering_e = propeller4_steering_sp - propeller4_steering_deg;

  //manual control
  if (steering_state4 == "left"){
  digitalWrite(propeller4_steering_pos, LOW);
  digitalWrite(propeller4_steering_neg, HIGH);
  } 
  
  if (steering_state4 == "right"){
  digitalWrite(propeller4_steering_pos, HIGH);
  digitalWrite(propeller4_steering_neg, LOW);
  }

  if(steering_state4 == "stop") {
  digitalWrite(propeller4_steering_pos, LOW);
  digitalWrite(propeller4_steering_neg, LOW);
  }
  
  
  
  //speed

  propeller4_speed_e = propeller4_speed_sp - rpm_propeller4;

  if (speed_state4 == "increase"){
  digitalWrite(propeller4_speed_pos, LOW);
  digitalWrite(propeller4_speed_neg, HIGH);
  } 
  
  if (steering_state4 == "right"){
  digitalWrite(propeller4_speed_pos, HIGH);
  digitalWrite(propeller4_speed_neg, LOW);
  }

  if(steering_state4 == "stop") {
  digitalWrite(propeller4_speed_pos, LOW);
  digitalWrite(propeller4_speed_neg, LOW);
  }
  
}



void gps_read(){
while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    
    Serial.println("No GPS detected");
    while(true);
  }  
}

void compass_read(){

  
}


void displayInfo() {
  if (gps.location.isValid())
  { 
    Serial.print("Latitude: ");
    latitude = gps.location.lat(), 6;
    Serial.println(latitude);
    
    longitude = gps.location.lng(), 6;
    Serial.print("Longitude: ");
    Serial.println(longitude);
    
    
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
   
  }


}


void battery_measure(){
  
}

void heat_measure(){
  
}

void wind_direction(){
  
}

void wind_speed(){
  
}
