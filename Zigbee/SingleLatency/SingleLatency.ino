#include <SoftwareSerial.h>
#include <XBee.h>

XBee trans = XBee();
XBee recv = XBee();
SoftwareSerial transSerial(2,3);
SoftwareSerial recvSerial(4,5);

#define len 10
/**** PARAMETERS ****/
uint16_t recvAddr = 0x1234;
uint8_t option = 1; // Acks disabled
uint8_t payload[len] = "ABCDEFGHI\0";
uint8_t id = 0;
/*******************/

Tx16Request tx = Tx16Request(recvAddr, option, payload, len, id);
Rx16Response rx = Rx16Response();

void setup() {
  Serial.begin(57600);
  transSerial.begin(57600);
  recvSerial.begin(57600);
  trans.setSerial(transSerial);
  recv.setSerial(recvSerial);
}

void loop() {
  trans.send(tx);
  unsigned long sendTime = millis();
  unsigned long recvTime = 0;

  recv.readPacket(5000);
  XBeeResponse &response = recv.getResponse();
  if(response.isAvailable()) {
    recvTime = millis();
  }

  if (recvTime == 0) {
    Serial.println("Didn't receive");
  } else {
    unsigned long latency = recvTime - sendTime;
    Serial.print("latency: ");
    Serial.println(latency);
  }
  delay(5000);
}
