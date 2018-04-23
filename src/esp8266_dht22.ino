/* Sketch to read a DHT22 sensor via a wemos D1 mini (esp8266 chip).
Connections (note: the numbers are DHT pins left to right in front face view):
  1. DHT VDD (datasheet: min 3V, typical 5V) --> Wemos 3.3V
  2. DHT DATA                                --> Wemos D0, also has a 10K res connected to DHT VDD
  3. NULL
  4. DHT GND                                 --> Wemos GND

Author: Nitish Lobo
Date: 19/06/17
*/

#include "Arduino.h"
#include "DHT.h"

#define DHTTYPE DHT22
#define DHTPIN D1

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  //Wait a few seconds between measurements.
  delay(2000);

  //Reading temperature or humidity takes about 250ms.
  //Sensor readings may also be up to 2 seconds 'old'.
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float hi = dht.computeHeatIndex(t, h);

  //Check for failure to read and exit loop early to try again.
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Print results to serial monitor.
  Serial.print("Temperature: "); Serial.print(t); Serial.print("*C");
  Serial.print("    ");
  Serial.print("Humidity: "); Serial.print(h); Serial.print("%");
  Serial.print("    ");
  Serial.print("Heat index: "); Serial.print(hi); Serial.println("*C");
}
