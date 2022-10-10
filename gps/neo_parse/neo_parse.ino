#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

// Choose two Arduino pins to use for software serial
int RXPin = 4;
int TXPin = 3;

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

unsigned long Time;
unsigned long Time_prev;;

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("GPS TEST");
  delay(2000);
  lcd.clear();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    lcd.setCursor(0,0);
    lcd.print("wiring teu baleg");
    Serial.println("No GPS detected");
    while(true);
  }
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    lcd.setCursor(0,0);
    lcd.print("lat:");
    
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    
    lcd.setCursor(5,0);
    lcd.print(gps.location.lat(), 6);
    
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    
    lcd.setCursor(0,1);
    lcd.print("lon:");
    
    lcd.setCursor(5,1);
    lcd.print(gps.location.lng(), 6);
    
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
    lcd.setCursor(0,0);
    lcd.print("No data");
    lcd.setCursor(0,1);
    lcd.print("No data");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }


  Time = millis() - Time_prev;
  if (Time > 8000){
    lcd.clear();
    Time_prev = millis();
  }
  Serial.println(Time);
}
