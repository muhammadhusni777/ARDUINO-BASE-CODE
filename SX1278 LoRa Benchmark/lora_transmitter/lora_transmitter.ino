#include <SPI.h>
#include <LoRa.h> 
int pot = A0;
int lat_send = 100;
int lon_send = 100;


void setup() {
  Serial.begin(9600);
  pinMode(pot,INPUT);
  
  while (!Serial);  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
  lat_send = random(0,100);
  lon_send = random(5,12);
  LoRa.beginPacket();  
  LoRa.print(lat_send);
  LoRa.print(",");
  LoRa.print(lon_send);
  LoRa.endPacket();
  Serial.print("data send : ");
  Serial.println(lat_send);
  delay(500);
 
}
