#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial mySerial(2,3);
XBee xbee = XBee();
//"This is a sentence that is exactly 48 bytes long. This sentence should bring the total to 100 bytes"

#define len 10
/**** PARAMETERS ****/
uint16_t addr16 = 0x1234;
uint8_t option = 1; // Acks disabled
uint8_t payload[len] = "This is a sentence that is exactly 48 bytes long. This sentence should bring the total to 100 bytes";
uint8_t id = 1;
/*******************/

Tx16Request tx = Tx16Request(addr16, option, payload, len, id);
TxStatusResponse rx = TxStatusResponse();

int numPackets;
int numSuccess;
int maxPackets = 500;
unsigned long prev;
unsigned long diff;

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  pinMode(13, OUTPUT);
  numPackets = 0;
  numSuccess = 0;
  prev = 0;
  diff = 0;
}

void loop() {
  
  if(numPackets < maxPackets) {
    prev = millis();
    digitalWrite(13, HIGH);
    xbee.send(tx);
    numPackets++;
    
    xbee.readPacket(10);
    if(xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      digitalWrite(13, LOW);
      xbee.getResponse().getTxStatusResponse(rx);
      if(rx.getErrorCode() == NO_ERROR) {
        numSuccess++;
      }
    }
  }
  else {
    Serial.println(numSuccess);
    delay(20000);
  }

  diff = millis();
  if(diff < prev + 10) {
    delay(prev + 10 - diff);
  }
}
