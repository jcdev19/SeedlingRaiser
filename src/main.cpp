#include <Arduino.h>
#include "Adafruit_Sensor.h"
#include "DHT.h"
#define DHTTYPE DHT11



#define DHTPIN 7

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(13, OUTPUT);
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

void getSoilMoisture() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.print("Analog Value : ");
  Serial.println(sensorValue);
  delay(1000);        // delay in between reads for stability
}

void waterPlants() {
  //turn on pump
}

void loop() {
  // put your main code here, to run repeatedly:
  getHumidityTemp();
  
  getSoilMoisture();

  //getSoilTemp();

  /*if getSoilMoisture < threshold {
    waterPlants();
    //run sooner if watered
    delay(5000);
  } else {
    //wait normal time
    delay(10000);
  } */

  
}