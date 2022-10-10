/*
mosquitto_pub -h 123.45.0.10 -p 1883 -t hoho -m *sudut servo*


*/
#include <Wire.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


unsigned long time_now;
unsigned long time_elapsed;
unsigned long time_prev;

unsigned long time_send;
unsigned long time_send_prev;
int time_message = 1000;

//MechaQMC5883 qmc;
int azimuth =0;

int val;
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
   val = messageTemp.toInt(); 
   Serial.println(val);
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
  Wire.begin();
  Serial.begin(115200);
  Serial.println("booting....");
  //qmc.init();
  client.setServer(server, 1883);
  client.setCallback(callback);


  pinMode (4, OUTPUT);
  
  
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
  //qmc.setMode(Mode_Continuous,ODR_200Hz,RNG_2G,OSR_256);

}

static char yaw_send[15];
void loop() {
  time_now = millis();
  
  if (!client.connected()) {
    reconnect();
  }
  

  time_send = millis() - time_send_prev;
  if (time_send > time_message){

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
  

}  
