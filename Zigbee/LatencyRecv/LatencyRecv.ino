#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial mySerial(2,3);
XBee xbee = XBee();

#define len 10
/**** PARAMETERS ****/
uint16_t addr16 = 0x5998;
uint8_t option = 1; // Acks disabled
uint8_t payload[len];
uint8_t id = 1;
/*******************/

Tx16Request tx = Tx16Request(addr16, option, payload, len, id);
TxStatusResponse rx = TxStatusResponse();

void waitForRequest() {
  Serial.println("Waiting for request...");

  //wait for request
  do {
    Serial.println("reading...");
    xbee.readPacket(5000);
  }while(!xbee.getResponse().isAvailable());
}

void serializeTime(unsigned long src, uint8_t dest[4]) {
  Serial.print("Raw Time ");
  Serial.println(src);
  
  dest[0] = src >> 24 & 0xFF;
  dest[1] = src >> 16 & 0xFF;
  dest[2] = src >> 8 & 0xFF;
  dest[3] = src >> 0 & 0xFF;

  Serial.print("Packet: ");
  for(int i = 0; i < 4; i++) {
    Serial.print(dest[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void syncWithTrans() {
  waitForRequest();

  unsigned long start = millis();

  uint8_t rawTime[4];
  serializeTime(start, rawTime);

  Tx16Request temp(addr16, option, rawTime, 4, 0);
  xbee.send(temp);
}

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  syncWithTrans();
}

void loop() {
  
}
