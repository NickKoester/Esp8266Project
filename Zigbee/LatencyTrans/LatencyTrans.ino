#include <SoftwareSerial.h>
#include <XBee.h>

XBee xbee = XBee();
SoftwareSerial mySerial(2,3);

#define len 10
/**** PARAMETERS ****/
uint16_t addr16 = 0x1234;
uint8_t option = 1; // Acks disabled
uint8_t payload[len];
uint8_t id = 1;
/*******************/

Tx16Request tx = Tx16Request(addr16, option, payload, len, id);
TxStatusResponse rx = TxStatusResponse();
Tx16Request rndmb = Tx16Request(addr16, option, payload, len, 0);

unsigned long syncMillisBase = 0;

void requestTimeFromRecv() {
  do{
    Serial.println("Sending...");
    xbee.send(rndmb);
    Serial.println("Waiting for reply");
    xbee.readPacket(5000);
  } while(!xbee.getResponse().isAvailable());
    
  Serial.println("got reply!");
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

void syncTimeWithRecv() {
  Serial.println("Started");

  unsigned long time1 = millis();
  requestTimeFromRecv();
  unsigned long time2 = millis();
  
  uint8_t *rawData = readPacketData();

  uint8_t rawTime[4];
  if(rawData) {
    memcpy(rawTime, rawData+4, 4);
  }
  
  unsigned long recvTime = getTimeFromArray(rawTime);

  syncMillisBase = recvTime + (time2 - time1) / 2; 
}

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  
  delay(342);

  syncTimeWithRecv();
  Serial.print("SyncMillisBase: ");
  Serial.println(syncMillisBase);
}

void loop() {
  
}
