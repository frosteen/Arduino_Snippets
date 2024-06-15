const int L0_NEG_PIN = 52; //Setup for leads off detection LO -
const int L0_POS_PIN = 53; // Setup for leads off detection LO +

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(L0_NEG_PIN, INPUT);
  pinMode(L0_POS_PIN, INPUT);
  delay(1000);
}

void loop() {
  if (!((digitalRead(L0_NEG_PIN) == 1) || (digitalRead(L0_POS_PIN) == 1))) {
    Serial.println(analogRead(A0));
  }
}
