#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial mySerial(2,3);
XBee xbee = XBee();
//"This is a sentence that is exactly 48 bytes long. This sentence should bring the total to 100 bytes"

#define len 10
/**** PARAMETERS ****/
uint16_t addr16 = 0x1234;
uint8_t option = 1; // Acks disabled
uint8_t payload[len] = "ABCDEFGHI\0";
uint8_t id = 0;
/*******************/

Tx16Request tx = Tx16Request(addr16, option, payload, len, id);

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  xbee.send(tx);
  digitalWrite(13, LOW);

  delay(6);
  delayMicroseconds(460);
}
