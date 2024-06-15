#include <SoftwareSerial.h>

#define RE 8
#define DE 7

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];
SoftwareSerial mod(2, 3);

void setup() {
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  delay(3000);
}

void loop() {
  byte val_nitrogen, val_phosphorous, val_potassium;
  val_nitrogen = nitrogen();
  delay(250);
  val_phosphorous = phosphorous();
  delay(250);
  val_potassium = potassium();
  delay(250);
  Serial.print("Nitrogen: ");
  Serial.print(val_nitrogen);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val_phosphorous);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val_potassium);
  Serial.println(" mg/kg");
  delay(2000);
}

byte nitrogen() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(nitro, sizeof(nitro)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
  }
  return values[4];
}

byte phosphorous() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(phos, sizeof(phos)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
  }
  return values[4];
}

byte potassium() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(pota, sizeof(pota)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
  }
  return values[4];
}
