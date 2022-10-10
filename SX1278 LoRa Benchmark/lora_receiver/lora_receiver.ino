#include <SPI.h>
#include <LoRa.h> 
int LED = 3;
String inString = "";    // string to hold input
char inChar = "";
String val = "";
 
void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet    
    while (LoRa.available())
    {
      inChar = LoRa.read();
      inString += (char)inChar;
      val = inString;//.toInt();       
    }
    inString = "";     
    LoRa.packetRssi();    
  }
  Serial.print("data received : ");
  Serial.println(val);  
}
