#include "DHT.h"

#define DHTPIN D2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float humidity_scale = 0.9375; // SLOPE
float humidity_offset = 2.3125; // OFFSET

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT22 Calibration!"));
  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT22 sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.println(h * humidity_scale + humidity_offset);
}
