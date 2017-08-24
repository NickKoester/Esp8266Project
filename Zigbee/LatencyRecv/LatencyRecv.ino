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

uint8_t *readPacketData() {
  uint8_t *data = NULL;
  
  XBeeResponse &response = xbee.getResponse();
  if(response.isAvailable()) {
    Serial.println("available");
    if(response.getApiId() == RX_16_RESPONSE) {
      Serial.println("Tx16");
      if(response.getErrorCode() == NO_ERROR) {
        Serial.println("no error");
        data = response.getFrameData();
      }
    }
  }
  return data;
}

unsigned long getTimeFromArray(uint8_t rawTime[4]) {
  Serial.print("Raw time: ");
  for(int i = 0; i < 4; i++) {
    Serial.print(rawTime[i], HEX);
    Serial.print(' ');
  }
  Serial.println();

  unsigned long num = 0;
  for(int i = 0; i < 4; i++) {
    num += rawTime[i] << ((3-i)*8);
  }

  Serial.print("Sync time in ms: ");
  Serial.println(num);
  
  return num;
}

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  syncWithTrans();
}

void loop() {
  xbee.readPacket(5000);
  unsigned long recvTime = millis();
  
  uint8_t *data = readPacketData();
  
  uint8_t rawTime[4];
  if(data) {
    memcpy(rawTime, data+4, 4);
  }
  
  unsigned long sendTime = getTimeFromArray(rawTime);
  unsigned long transmitLatency = recvTime - sendTime;

  Serial.print("Transmit Latency: ");
  Serial.println(transmitLatency);
  delay(1000);
}
