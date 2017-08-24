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

uint8_t *rawData;
uint8_t rawTime[4];
unsigned long start;
unsigned long sync = 0;
void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  
  delay(342);
  Serial.println("Started");

  start = millis();


  bool gotit = false;
  while(!gotit) {
    Serial.println("Sending...");
    xbee.send(rndmb);

    do{
      Serial.println("Waiting for reply");
      xbee.readPacket(5000);
    } while(!xbee.getResponse().isAvailable());

    Serial.println("got reply!");
    
    XBeeResponse &response = xbee.getResponse();
    if(response.isAvailable()) {
      Serial.println("available");
      if(response.getApiId() == RX_16_RESPONSE) {
        Serial.println("Tx16");
        if(response.getErrorCode() == NO_ERROR) {
          Serial.println("no error");
          rawData = response.getFrameData();
          gotit = true;
        }
      } else {
        Serial.print("api id = ");
        Serial.println(response.getApiId());
      }
    }
  }

  memcpy(rawTime, rawData+4, 4);
  
  Serial.print("Raw time: ");
  for(int i = 0; i < 4; i++) {
    Serial.print(rawTime[i], HEX);
    Serial.print(' ');
  }
  Serial.println();

  for(int i = 0; i < 4; i++) {
    sync += rawTime[i] << ((3-i)*8);
  }

  Serial.print("Sync time in ms: ");
  Serial.println(sync);
}

void loop() {
  
}
