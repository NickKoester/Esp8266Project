#include <SoftwareSerial.h>
#include <XBee.h>

XBee xbee = XBee();
SoftwareSerial mySerial(10,11);

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

uint8_t *rawTime;
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
    
    xbee.readPacket(5000);
    XBeeResponse &response = xbee.getResponse();
    if(response.isAvailable()) {
      if(response.getApiId() == TX_16_REQUEST) {

        if(response.getErrorCode() == NO_ERROR) {
          rawTime = response.getFrameData();
          gotit = true;
        }
      }
    }
  }
  
  Serial.print("Raw time: ");
  for(int i = 0; i < len; i++) {
    Serial.print(rawTime[i], HEX);
    Serial.print(' ');
  }
  Serial.println();

  for(int i = 0; i < 4; i++) {
    sync += rawTime[3-i] * pow(256,i);
  }

  Serial.print("Sync time in ms: ");
  Serial.println(sync);
}

void loop() {
  
}
