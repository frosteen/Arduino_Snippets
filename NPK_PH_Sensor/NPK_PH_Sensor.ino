#include <SoftwareSerial.h>

// NPK
#define RE 8
#define DE 7
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
SoftwareSerial mod(2, 3);

// PH
float calibration_value = 15.82;

void setup() {
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  delay(3000);
}

void loop() {
  float val_ph;
  byte val_nitrogen, val_phosphorous, val_potassium;
  val_ph = ph();
  delay(250);
  val_nitrogen = nitrogen();
  delay(250);
  val_phosphorous = phosphorous();
  delay(250);
  val_potassium = potassium();
  delay(250);
  Serial.print(val_ph);
  Serial.print(",");
  Serial.print(val_nitrogen);
  Serial.print(",");
  Serial.print(val_phosphorous);
  Serial.print(",");
  Serial.println(val_potassium);
  delay(2000);
}

float ph() {
  static unsigned long int avgval;
  static int buffer_arr[10], temp;
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6;
  float ph_act = -5.70 * volt + calibration_value;
  return ph_act;
}

byte nitrogen() {
  static byte values[11];
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
  static byte values[11];
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
  static byte values[11];
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
