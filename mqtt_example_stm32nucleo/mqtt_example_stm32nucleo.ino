#include <Servo.h>
Servo servo;
int pos = 0;
/*
mosquitto_pub -h 123.45.0.10 -p 1883 -t hoho -m *sudut servo*


*/
#include <Wire.h>

#include <PubSubClient.h>
#include <LwIP.h>
#include <STM32Ethernet.h>

#include<IWatchdog.h>


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
    pos = messageTemp.toInt(); 
   
   }
   
  if (String(topic) == "hoho") {
    Serial.print("Changing output to ");
    if(messageTemp == "1on"){
      Serial.println("on");
      digitalWrite(LED_RED, HIGH);
    }
    else if(messageTemp == "1off"){
      Serial.println("off");
      digitalWrite(LED_RED, LOW);
    }
    else if(messageTemp == "2on"){    
      Serial.println("off");  
      digitalWrite(LED_GREEN, HIGH);
    }
     else if(messageTemp == "2off"){
      Serial.println("off");
      digitalWrite(LED_GREEN, LOW);
    }
     else if(messageTemp == "3on"){    
      Serial.println("off");  
      digitalWrite(LED_BLUE, HIGH);
    }
     else if(messageTemp == "3off"){
      Serial.println("off");
      digitalWrite(LED_BLUE, LOW);
    }
    else if(messageTemp == "4on"){    
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
  servo.attach(D6);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("booting....");
  //qmc.init();
  client.setServer(server, 1883);
  client.setCallback(callback);

  pinMode (LED_RED, OUTPUT);
  pinMode (LED_BLUE, OUTPUT);
  pinMode (LED_GREEN, OUTPUT);
  pinMode (4, OUTPUT);
  
  
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
  //qmc.setMode(Mode_Continuous,ODR_200Hz,RNG_2G,OSR_256);
  IWatchdog.begin(4000000);
}

static char yaw_send[15];
void loop() {
   servo.write(pos);
  time_now = millis();
  
  if (!client.connected()) {
    reconnect();
  }
  
  int x, y, z;
  //double azimuth;
  
//  float azimuth; //is supporting float too
  //qmc.read(&x, &y, &z,&azimuth);
//  azimuth = qmc.azimuth(&y,&x);//you can get custom azimuth
  time_send = millis() - time_send_prev;
  if (time_send > time_message){
  Serial.print(pos);
  Serial.print(" dt :");
  Serial.print(time_elapsed);
 
  client.publish("Yaw",dtostrf(azimuth,6,3,yaw_send)); // kirim data
  //client.publish("Yaw, dtostrf(100));
  Serial.println();
  time_send_prev = millis();
  }
  
  
  client.loop();
  time_elapsed = time_now - time_prev;
  time_prev = time_now;
  
  IWatchdog.reload();
}  
