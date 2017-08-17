#include <SoftwareSerial.h>
#include <XBee.h>

XBee xbee = XBee();
SoftwareSerial mySerial(2,3);

/**** PARAMETERS ****/
float timeInterval = 10.0;
/*******************/

/**** GLOBALS ****/
Rx16Response rx = Rx16Response();

int recvPackets;
unsigned long prev;
/****************/

void benchOutput() {
  //Output throughtput in Kb/s
  Serial.println(recvPackets);
}

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  pinMode(13, OUTPUT);
  recvPackets = 0;
  prev = 0;
}

void loop() {
  xbee.readPacket();

  XBeeResponse &response = xbee.getResponse();
  if(response.isAvailable()) {
    digitalWrite(13, HIGH);
    if(response.getApiId() == RX_16_RESPONSE) {

      if(response.getErrorCode() == NO_ERROR) {
        ++recvPackets;
      }
      
      digitalWrite(13, LOW);
    }
  }

  if(millis() > prev + timeInterval * 1000) {
    benchOutput();
    prev = millis();
  }
}
