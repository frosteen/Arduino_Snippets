// include the SD library:
#include <SPI.h>
#include <SD.h>

#define CS_SD 10

File file;

void setup() {
  Serial.begin(9600);

  // INITIATE SD CARD MODULE
  if (!SD.begin(CS_SD)) {
    Serial.println("SD CARD MODULE FAILED.");
    return;
  }

  // SPI SELECT SD CARD MODULE
  digitalWrite(CS_SD, LOW);

  // FILE REMOVE
  String filename = "Logs.txt";
  if (SD.exists(filename)) {
    SD.remove(filename);
  }

  // WRITE FILE
  file = SD.open(filename, FILE_WRITE);
  if (file) {
    file.print("1,2,3,4,5");
    file.close();
  }

  // READ FILE
  file = SD.open(filename);
  if (file) {
    while (file.available()) {
      Serial.print((char)file.read());
    }
    file.close();
  }

  // SPI DESELECT SD CARD MODULE
  digitalWrite(CS_SD, HIGH);
}

void loop() {

}
