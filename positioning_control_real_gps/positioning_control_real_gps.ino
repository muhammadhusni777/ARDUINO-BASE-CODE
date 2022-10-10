#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,10); // RX, TX
String myString;

String last_message;

String buffer_message;
char a;

int Index1,Index2,Index3,Index4,Index5,Index6, Index7;
String secondValue, thirdValue, fourthValue, fifthValue, sixthValue, firstValue;
float longitude, latitude, longitude_target, latitude_target, latitude_error, longitude_error;

float ship_x, ship_y;
float error_degree;
unsigned long data_transmit_time;
unsigned long data_transmit_time_prev;



bool stringComplete = false;

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#define delimiter ' '
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xE6 }; //0x00, 0x80, 0xE1, 0x01, 0x01, 0x01
//byte mac[]    = {  0x00, 0x80, 0xE1, 0x01, 0x01, 0x01 };

IPAddress ip(123, 45, 0, 105);
IPAddress server(123, 45, 0, 10);

String position_lock_state = "deactive";

String inputString = "";         // a string to hold incoming data
String splitString = "";         // a string to parsing incoming data
//boolean stringComplete = false;  // whether the string is complete
boolean stringStart = false;

char msg[5];
int count = 0;

//float latitude = 0;
String lat,longi;
int longstr,b = 0;



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
    if (String(topic) == "position_lock") {
    Serial.print(messageTemp);
    position_lock_state = messageTemp;
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
      client.subscribe("position_lock");

      
      client.subscribe("steering1");
      client.subscribe("steering2");
      client.subscribe("steering3");
      client.subscribe("steering4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  } 
}


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(38400);
  Serial1.begin(38400);

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  splitString.reserve(50);
  
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port
 client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.init(10);  
  Ethernet.begin(mac, ip);
}


static char latitude_send[15];
static char longitude_send[15];
static char latitude_target_send[15];
static char longitude_target_send[15];
static char latitude_error_send[15];
static char longitude_error_send[15];
static char ship_x_send[15];
static char ship_y_send[15];

String latt =  "-6.000691";
String longg =  "106.6932";
String yaw = "1.5";
static char lat_out[13];
static char lat_send[14];
String lat_,long_;
float lat__,long__;
static char long_out[14];
static char long_send[14];
char yaw_out[100];


void loop() // run over and over
{
 if (!client.connected()) {
    reconnect();
  }
//  serialEvent();
  //dummy *6.8915|107.61087#
  data_transmit_time = millis() - data_transmit_time_prev;

  //count lat and long error in meter
  latitude_error = (latitude - latitude_target) * 111000;
  longitude_error = (longitude - longitude_target) * 111000;

  error_degree = acos(abs(latitude_error/longitude_error))*180/PI;
  
  ship_x = map(longitude_error, 0, 2000, 187, 370);
  ship_y = map(latitude_error, -2000, 0, 200, 98);
  if (ship_x > 370){
    ship_x = 370;
  }
  if (ship_x < 0){
    ship_x = 0;
  }

  if (ship_y > 200){
    ship_y = 200;
  }
  if (ship_y < 0){
    ship_y = 0;
  }

  
  if(position_lock_state == "deactive"){
    latitude_target = latitude;
    longitude_target = longitude;  
  }
  
  
  
  if (data_transmit_time > 1000){
  Serial.print("lat:");Serial.print(latitude,5);
  Serial.print("    lat target:");Serial.println(latitude_target,5);
  
  Serial.print("long:");Serial.print(longitude,5); 
  Serial.print("    long target:");Serial.println(longitude_target,5); 

  Serial.println(error_degree);

  client.publish("lat",dtostrf(latitude,6,3,latitude_send)); // kirim data
  client.publish("long",dtostrf(longitude,6,3,longitude_send)); // kirim data
  client.publish("lat_target",dtostrf(latitude_target,6,3,latitude_target_send)); // kirim data
  client.publish("long_target",dtostrf(longitude_target,6,3,longitude_target_send)); // kirim data

  client.publish("lat_error",dtostrf(latitude_error,6,3,latitude_error_send)); 
  client.publish("long_error",dtostrf(longitude_error,6,3,longitude_error_send)); 
  client.publish("ship_x",dtostrf(ship_x,6,3,ship_x_send)); 
  client.publish("ship_y",dtostrf(ship_y,6,3,ship_y_send)); 
  data_transmit_time_prev = millis();
  }
  client.loop();
 }
 
//dummy data only serialEvent


void serialEvent() {
  while (Serial.available()) {
  if (Serial.available() > 0){
    a = Serial.read();
    myString += String(a);
    if (String(a) == "#"){
      Serial.println(myString);
      if (myString.length()>0) {
      Index1 = myString.indexOf('*');
      Index2 = myString.indexOf('|', Index1+1);
      Index3 = myString.indexOf('|', Index2+1);
      Index4 = myString.indexOf('|', Index3+1);
      Index5 = myString.indexOf('|', Index4+1);
      Index6 = myString.indexOf('|', Index5+1);
      Index7 = myString.indexOf('|', Index6+1);
      secondValue = myString.substring(Index1+1, Index2);
      thirdValue = myString.substring(Index2+1, Index3);
      fourthValue = myString.substring(Index3+1, Index4);
      fifthValue = myString.substring(Index4+1, Index5);
      sixthValue = myString.substring(Index5+1, Index6);
      firstValue = myString.substring(Index6+1, Index7);
      //Serial.println(Index1);
      latitude = secondValue.toFloat();
      
      longitude = thirdValue.toFloat();
      
      //expiration = fourthValue.toFloat();      
      myString = "";
      }
     }  
    }
  }
}

// MQTT Topic
const char* latitudePublish = "GPS/lat";
const char* longitudePublish = "GPS/long";
const char* stringPublish = "GPS/debug/string";
const char* splitPublish = "GPS/debug/split";
const char* lengthPublish = "GPS/debug/length";
const char* mainSubscribe = "MainControl";



void serialEvent1(){
  if (Serial1.available()) {
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    splitString += inChar;
    
    if(inChar=='\n' || inChar==delimiter){
      if(!(splitString.charAt(0)==delimiter && splitString.length())){
        count += 1;
//        client.publish(splitPublish,splitString.c_str());
      }

      if(count==1){
        if (splitString.charAt(4)=='/' && splitString.charAt(7)=='/')
          stringStart = true;
        else
          stringStart = false;
      }
      
      if(stringStart){        
        if(count==3){
          lat = splitString; //.c_str()
          lat.replace(" ","");
          lat.toCharArray(lat_out,14);
//          lat_ = lat_out;          
          a++;
          if (a>3){
           a=50;
           client.publish(latitudePublish,lat_out); //dtostrf(test,12,9,lat_out)
          }
//          client.publish(latitudePublish,lat_out); //dtostrf(test,12,9,lat_out)
//          lat_ = lat_out;
//          client.publish(latitudePublish,dtostrf(lat_.toFloat(),12,9,lat_send)); //dtostrf(test,12,9,lat_out)
//          Serial.print(lat_out);
//          Serial.print("\t");

        }else if(count==4){
          longi = splitString; //.c_str()
          longi.toCharArray(long_out,14);
          b++;
          if (b>3){
            b=50;
            client.publish(longitudePublish,long_out); //,dtostrf(longitude.toFloat(),12,9,long_out)
          }
//          long_ = long_out;
//          client.publish(longitudePublish,dtostrf(long_.toFloat(),12,9,long_send)); //,dtostrf(longitude.toFloat(),12,9,long_out)
//          Serial.println(long_out);
          stringComplete = true;
          count = 0;
        }
      }

      splitString = "";
      
      if (inChar=='\n'){ 
         stringComplete = true;
         count = 0;
      }
    }
  }
  
}
