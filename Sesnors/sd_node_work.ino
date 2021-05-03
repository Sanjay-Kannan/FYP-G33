#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// pino de dados do DHT será ligado no D6 do esp
#define DHTPIN D2
// tipo do sensor
#define DHTTYPE DHT11

// construtor do objeto para comunicar com o sensor
DHT dht(DHTPIN, DHTTYPE);

//pino ligado ao CS do módulo SD Card
#define CS_PIN  D8

boolean alreadyRun = false;
float humidity;
float temperature;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  //inicializa o objeto para comunicarmos com o sensor DHT
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
}

void loop()
{
  read_dht();
  //rtc_mod();
  write_sd();
  
  delay(5000);
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
    dataFile.print("Date,Time,Temperature,Humidity,Vibration"); //Write the first row of the excel file
    dataFile.println();
    dataFile.close();
   }
   alreadyRun = true;
}


void write_sd()
{
  File dataFile = SD.open("LOG.csv", FILE_WRITE);
  DateTime now = rtc.now();
// se o arquivo foi aberto corretamente, escreve os dados nele
  if (dataFile) {
  Serial.println("The file was successfully opened.");
  //formatação no arquivo: linha a linha >> UMIDADE | TEMPERATURA
    //dataFile.print("20/4/2021");
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
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
    dataFile.print("Nil");
    dataFile.print(",");
    dataFile.println(); //End of Row move to next row
    dataFile.close();
  }
// se o arquivo não pôde ser aberto os dados não serão gravados.
  else {
  Serial.println("Failed to open LOG.csv file");
}
//intervalo de espera para uma nova leitura dos dados.
  delay(2000);
}

void read_dht() 
{
  humidity = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humidity);
//faz a leitura da temperatura
  temperature = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temperature);
}

/*void rtc_mod()
{
    DateTime now = rtc.now();
    
    Serial.println("Current Date & Time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}*/
