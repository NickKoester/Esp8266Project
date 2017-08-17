#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial mySerial(10,11);
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

uint8_t rawTime[4];
unsigned long start;
unsigned long sync = 0;
void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  
  Serial.println("Waiting for request...");

  bool gotit = false;
  while(!gotit) {
    xbee.readPacket(5000);
    if(xbee.getResponse().isAvailable()) {
      gotit = true;
    }
    else {
      Serial.println("didnt get it");
    }
  }

  start = millis();
  
  Serial.print("Start time in ms: ");
  Serial.println(start);
  
  rawTime[0] = start >> 24 & 0xFF;
  rawTime[1] = start >> 16 & 0xFF;
  rawTime[2] = start >> 8 & 0xFF;
  rawTime[3] = start >> 0 & 0xFF;

  Serial.print("Packet: ");
  for(int i = 0; i < 4; i++) {
    Serial.print(rawTime[i], HEX);
    Serial.print(' ');
  }
  Serial.println();

  Tx16Request temp(addr16, option, rawTime, 4, id);
  xbee.send(temp);
  
}

void loop() {
  
}
