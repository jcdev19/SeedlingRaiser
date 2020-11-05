#include <Arduino.h>
#include "Adafruit_Sensor.h"
#include "DHT.h"
#define DHTTYPE DHT11



const int DHT_PIN = 7;
const int RELAY_PIN = 3;
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  dht.begin();
}

void getHumidityTemp() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

   if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Air Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  delay(1000);
}

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
}

void loop() {
  // put your main code here, to run repeatedly:
  getHumidityTemp();
  
  int soilMoisture = getSoilMoisture();

  if (soilMoisture > 645) {
    waterPlants();
    //run sooner if watered
    delay(5000);
  } else {
    //wait normal time
    Serial.println("DROWNING!!!");
    delay(10000);
  }




  

  
}