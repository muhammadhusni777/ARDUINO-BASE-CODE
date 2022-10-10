char array[] = "-6.914744,107.609810";

char *strings[10]; 
char *ptr = NULL;


float latitude;
float longitude;

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

   Serial.println("The Pieces separated by strtok()");
   for (int n = 0; n < index; n++)
   {
      Serial.print(n);
      Serial.print("  ");
      Serial.println(strings[n]);

      latitude = atof(strings[0]); 
      longitude = atof(strings[1]);
   }
   
   

   Serial.print("\nlat = ");
   Serial.println(latitude, 5);
   Serial.print("long = ");
   Serial.println(longitude, 5);
   
}

void loop()
{

}
