#include <SoftwareSerial.h>

SoftwareSerial AT_Serial(2, 3); // RX_OF_ARDUINO, TX_OF_ARDUINO

void setup() {
  Serial.begin(9600); // SERIAL MONITOR
  AT_Serial.begin(9600); // HC05_Bluetooth
  Serial.println("Begin Sending AT/Text Commands.");
}

void loop() {
  // TRANSMIT DATA FROM SERIAL MONITOR TO BLUETOOTH
  while (Serial.available()) {
    AT_Serial.write(Serial.read());
  }

  // RECEIVE DATA FROM BLUETOOTH TO SERIAL MONITOR
  while (AT_Serial.available()) {
    Serial.write(AT_Serial.read());
  }
}
