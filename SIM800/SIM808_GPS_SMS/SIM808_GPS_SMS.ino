#include <SoftwareSerial.h>

#define PIN_TX_OF_SIM808 D4
#define PIN_RX_OF_SIM808 D3
String phone_number = "+639476207065";
SoftwareSerial sim808(PIN_TX_OF_SIM808, PIN_RX_OF_SIM808);

void send_command(SoftwareSerial& serial, String command) {
  serial.println(command);
  delay(100);
  while (serial.available())
    serial.read();
}

void send_command(SoftwareSerial& serial, char command) {
  serial.println(command);
  delay(100);
  while (serial.available())
    serial.read();
}

void init_sim808() {
  while (!sim808.available()) {
    sim808.println("AT");
    delay(1000);
    Serial.println("Connecting SIM808...");
  }
  Serial.println("Connected SIM808!");

  while (!sim808.available()) {
    sim808.println("AT+CMGF=1");
    delay(1000);
    Serial.println("Checking GSM...");
  }
  Serial.println("Successful GSM!");

  while (!sim808.available()) {
    sim808.println("AT+CGNSPWR=1");
    delay(1000);
    Serial.println("Checking GPS...");
  }
  Serial.println("Successful GPS!");

  while (!sim808.available()) {
    sim808.println("AT+CGNSSEQ=RMC");
    delay(1000);
    Serial.println("Checking GPS_RMC...");
  }
  Serial.println("Successful GPS_RMC!");
}

void send_sms(String message) {
  send_command(sim808, "AT+CMGF=1");
  send_command(sim808, "AT+CMGS=\"" + phone_number + "\"");
  send_command(sim808, message);
  send_command(sim808, (char)26);
}

String convertTimestamp(String timestamp, int timezoneOffsetHours) {
  // Extract year, month, day, hour, minute, second
  String year = timestamp.substring(0, 4);
  String month = timestamp.substring(4, 6);
  String day = timestamp.substring(6, 8);
  String hour = timestamp.substring(8, 10);
  String minute = timestamp.substring(10, 12);
  // Convert strings to integers
  int yearInt = year.toInt();
  int monthInt = month.toInt();
  int dayInt = day.toInt();
  int hourInt = hour.toInt();
  int minuteInt = minute.toInt();
  // Adjust for timezone offset
  int totalMinutes = (hourInt * 60) + minuteInt + (timezoneOffsetHours * 60);
  int newHour = totalMinutes / 60;
  int newMinute = totalMinutes % 60;
  // Check if the new hour exceeds 24 hours
  if (newHour >= 24) {
    // Handle the case where the time exceeds 24 hours
    // For example, you might want to adjust the day and reset the hour
    dayInt += newHour / 24;
    newHour = newHour % 24;
  }
  // Determine AM/PM before adjusting for 12-hour format
  String ampm = (newHour >= 12) ? "PM" : "AM";
  // Convert hour to 12-hour format
  int calculatedHour = newHour;
  if (calculatedHour > 12) {
    calculatedHour -= 12;
  } else if (calculatedHour == 0) {
    calculatedHour = 12;
  }
  // Format the date and time
  char formattedDate[20];  // Ensure this is large enough for your formatted string
  sprintf(formattedDate, "%02d/%02d/%04d %02d:%02d %s", monthInt, dayInt, yearInt, calculatedHour, newMinute, ampm.c_str());
  return String(formattedDate);
}

String* get_gps() {
  // data[2] - timestamp
  // data[3] - latitude
  // data[4] - longitude
  // data[5] - altitude
  const int data_size = 6;
  static String data[data_size];
  int i = 0;

  for (int i = 0; i < data_size; i++)
    data[i] = "";

  send_command(sim808, "AT+CGNSPWR=1");

  send_command(sim808, "AT+CGNSSEQ=RMC");

  sim808.println("AT+CGNSINF");
  delay(100);
  while (sim808.available() && i < data_size) {
    // Serial.print(sim808.readString());
    char c = sim808.read();
    if (c != ',')
      data[i] += c;
    else
      i++;
  }
  return data;
}

void setup() {
  sim808.begin(9600);
  Serial.begin(115200);
  init_sim808();
}

void loop() {
  String* data = get_gps();
  String get_data = *(data + 1);
  if (get_data != "") {
    String timestamp = *(data + 2);
    String latitude = *(data + 3);
    String longitude = *(data + 4);
    String altitude = *(data + 5);
    Serial.print("TIME:");
    Serial.println(convertTimestamp(timestamp, 8));
    Serial.print("LAT:");
    Serial.println(latitude);
    Serial.print("LON:");
    Serial.println(longitude);
    Serial.print("ALT:");
    Serial.println(altitude);
  }
}
