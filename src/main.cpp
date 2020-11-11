#include <Arduino.h>
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include <OneWire.h>
#define DHTTYPE DHT11


const int DS18S20_Pin = 10; //DS18S20 Signal pin on digital 7
const int DHT_PIN = 7;
const int RELAY_PIN = 4;

DHT dht(DHT_PIN, DHTTYPE);
OneWire ds(DS18S20_Pin);

String AP = "Telstra68C665";       // CHANGE ME
String PASS = "z6z74gvvgt"; // CHANGE ME
String API = "Telstra68C665";   // CHANGE ME
String HOST = "192.168.0.82";
String PORT = "3000";
String field = "field1";


int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    Serial1 .println(command);//at+cipsend
    if(Serial1.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  dht.begin();
  Serial1.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  delay(10000);
}

float getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius
 
 byte data[12];
 byte addr[8];
 
 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return -1000;
 }
 
 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }
 
 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }
 
 ds.reset();
 ds.select(addr);
 ds.write(0x44,1); // start conversion, with parasite power on at the end
 
 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Read Scratchpad
 
 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds.read();
 }
 
 ds.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];
 
 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;
 
}


float getHumidity() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();

   if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Air Temperature: "));
  // Serial.print(t);
  // Serial.println(F("°C "));
  return h;
  delay(1000);
}
float getAirTemp() {
  // float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

   if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 1.0;
  }

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Air Temperature: "));
  // Serial.print(t);
  // Serial.println(F("°C "));
  return t;
  delay(1000);
}

// int getSoilTempurature() {
//   return 19;
// }

int getSoilMoisture() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.print("Soil Moisture Value : ");
  Serial.println(sensorValue);
  return sensorValue;
}

void waterPlants() {
  //turn on pump
  Serial.println("Watering Plants");
  digitalWrite(RELAY_PIN, HIGH);
  //watering for 30 seconds
  delay(5000);
  digitalWrite(RELAY_PIN, LOW);

}

void loop() {
  
  // put your main code here, to run repeatedly:
  float airTemperature = getAirTemp();
  float humidity = getHumidity();
  
  int soilMoisture = getSoilMoisture();
  //int soilTemperature = getSoilTempurature();
  float soilTemperature = getTemp();
  Serial.print("SOILtemperature");
  Serial.println(soilTemperature);
  boolean watered;
  if (soilMoisture > 400 && soilTemperature > 15) {
    waterPlants();
    //run sooner if watered 
    //wait five minutes
    delay(300000);
    // delay(5000);
    watered = true;
    // delay(2000);
  } else {
    Serial.println("DROWNING!!!");
    //wait half hour
    delay(1800000);
    // delay(5000);
    watered = false;
  }

  // String getData = "PUT /test/{soilMoist:"+String(soilMoisture)+",soilTemp:" + String(soilTemperature) + "} HTTP/1.1";
  String getData = "GET /data/airTemp/"+String(airTemperature)+"/soilTemp/"+String(soilTemperature)+"/humidity/"+String(humidity)+"/soilMoisture/"+String(soilMoisture)+"/watered/"+String(watered)+" HTTP/1.1";
  String getHost = "Host: 192.168.0.82";
  Serial.println(getData);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+getHost.length()+6),4,">");
 Serial1.println(getData);delay(1500);countTrueCommand++;
 Serial1.println(getHost);delay(1500);countTrueCommand++;
 Serial1.println();
 Serial1.println();
 sendCommand("AT+CIPCLOSE=0",5,"OK");

}