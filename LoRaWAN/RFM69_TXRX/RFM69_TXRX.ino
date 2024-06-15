#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 434.0

// Where to send packets to!
#define DEST_ADDRESS   255
// change addresses for each client board, any number :)
#define MY_ADDRESS     2

// Pinouts
#define RFM69_CS      A3
#define RFM69_RST     A2
#define RFM69_IRQ     2

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_IRQ);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

void setup() {
  Serial.begin(9600);

  // set reset
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  // init RF69
  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  } else Serial.println("RFM69 radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                  };
  rf69.setEncryptionKey(key);

  // print details
  Serial.println("RFM69 radio @" + String((int)RF69_FREQ) + String(" MHz"));
}

void do_transmit(String message) {
  // convert string to char array
  char radiopacket[50];
  message.toCharArray(radiopacket, 50);
  Serial.println("Sending: " + String(radiopacket));
  // Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), DEST_ADDRESS)) {
    Serial.println("Sent");
  }
}

void do_receive() {
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  if (rf69_manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAck(buf, &len, &from)) {
      buf[len] = 0; // zero out remaining string
      Serial.println("Got packet from #" + String(from));
      Serial.println("RSSI: " + String(rf69.lastRssi()));
      Serial.println("Message: " + String((char*)buf));
    }
  }
}

void loop() {
  delay(1000);
  String message = "Hello World";
  do_transmit(message);
  do_receive();
}
