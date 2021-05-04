#include <SoftwareSerial.h>
#include <TinyGPS.h>
float lat,lon;                          // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(9,8);         //tx,rx
TinyGPS gps;                          // create gps object
void setup()
{
Serial.begin(9600);                 // connect serial
gpsSerial.begin(9600);             // connect gps sensor
}
 
void loop()
{
    while(gpsSerial.available())
    {                                          // check for gps data
    if(gps.encode(gpsSerial.read()))          // encode gps data
    { 
    gps.f_get_position(&lat,&lon);            // get latitude and longitude
     String latitude = String(lat,6);
    String longitude = String(lon,6);   
    Serial.println("https://www.google.com/maps/?q="+latitude+","+longitude);
    delay(1000);  
   }
  } 
}
