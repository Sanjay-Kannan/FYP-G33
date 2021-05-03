#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define DHTPIN D3
#define vib_pin D0
#define ss D4
unsigned int t=0,RPM=0;
// tipo do sensor
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define CS_PIN  D8

boolean alreadyRun = false;
float humidity;
float temperature;
int val;
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  dht.begin();
  if (alreadyRun == false)
  {
    init_sd();
  }
  else 
  Serial.println("Still running loop()"); 

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  }
  Initialize_RTC();
/*  Wire.begin(9,10);
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);*/
}

void loop()
{
  read_dht();
  Vib();
  spd();
  //rtc_mod();
  //gyr();
  write_sd();
  
  delay(5000);
}
void Initialize_RTC()
{
   // Initialize the rtc object
  Wire.begin();

}
void init_sd()
{
  SD.begin(CS_PIN);
  if (!SD.begin(CS_PIN))
  {
    
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
   }
   // open the file. note that only one file can be open at a time,
   // so you have to close this one before opening another.
   File dataFile = SD.open("LoggerCD.csv", FILE_WRITE);
   // if the file is available, write to it:
   if (dataFile)
   {
    dataFile.print("Date,Time,Temperature,Humidity,Vibration,Speed,X,Y,Z"); //Write the first row of the excel file
    dataFile.println();
    dataFile.close();
   }
   alreadyRun = true;
}


void write_sd()
{
  File dataFile = SD.open("LoggerCD.csv", FILE_WRITE);
  DateTime now = rtc.now();
  //if (dataFile) {
  Serial.println("The file was successfully opened.");
    //dataFile.print("20/4/2021");
    dataFile.print(now.day());
    dataFile.print("|");
    dataFile.print(now.month());
    dataFile.print("|");
    dataFile.print(now.year());
    dataFile.print(",");
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print(",");
    dataFile.print(temperature); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","
    dataFile.print(humidity); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","
    dataFile.print(val);
    dataFile.print(",");
    dataFile.print(RPM);
    dataFile.print(",");
    dataFile.print(x);
    dataFile.print(",");
    dataFile.print(y);
    dataFile.print(",");
    dataFile.print(z);
    dataFile.print(",");
    dataFile.println(); //End of Row move to next row
    dataFile.close();
    Serial.print(now.day() , DEC);
    Serial.print(':');
    Serial.print(now.month(), DEC);
    Serial.print(':');
    Serial.print(now.year(), DEC);
  //}
 /* else {
  Serial.println("Failed to open LoggerCD.csv file");
}*/
  delay(2000);
}

void read_dht() 
{
  humidity = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humidity);
  temperature = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temperature);
}
void Vib()
{
  val=digitalRead(vib_pin);
}
int delay1()
{
  //unsigned int long k;
    int i,j;
    unsigned int count=0;
    for(i=0;i<1000;i++) 
    {
      for(j=0;j<1227;j++)
      {
       if(digitalRead(ss)==LOW)
       {
         count++;
       }
    }
  }
    return count;
}
void spd(){
  t=delay1();
     RPM=(t/789);
}
/*void gyr()
{
  Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");

}*/
