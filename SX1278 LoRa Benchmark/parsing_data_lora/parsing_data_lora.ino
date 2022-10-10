char array[] = "0,71";

char *strings[6]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;

int Sunlight;
int Humidity;
int Temperature;
float Pressure;
int Dewpoint;

void setup()
{
   Serial.begin(115200);
   byte index = 0;
   ptr = strtok(array, ",");  // delimiter
   while (ptr != NULL)
   {
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");
   }
   //Serial.println(index);
   // print all the parts
   Serial.println("The Pieces separated by strtok()");
   for (int n = 0; n < index; n++)
   {
      Serial.print(n);
      Serial.print("  ");
      Serial.println(strings[n]);
   }
   //Those values are Sunlight,Humidity,Temperature,Pressure,Dewpoint.
  // use the atoi() and atof() functions to convert ASCII strings to numbers.
   Sunlight = atoi(strings[0]); //http://www.cplusplus.com/reference/cstdlib/atoi/?kw=atoi
   Humidity = atoi(strings[1]);
   Temperature = atoi(strings[2]);
   Pressure = atof(strings[3]); //http://www.cplusplus.com/reference/cstdlib/atof/?kw=atof
   Dewpoint = atoi(strings[4]);

   Serial.print("\nSunlight = ");
   Serial.println(Sunlight);
   Serial.print("Humidity = ");
   Serial.println(Humidity);
   Serial.print("Temperature = ");
   Serial.println(Temperature);
   Serial.print("Pressure = ");
   Serial.println(Pressure);
   Serial.print("Dewpoint = ");
   Serial.println(Dewpoint);

}

void loop()
{

}
