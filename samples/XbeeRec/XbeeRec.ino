#include <SoftwareSerial.h>
#include <XBee.h>

XBee xbee = XBee();
SoftwareSerial mySerial(2,3);

/**** PARAMETERS ****/
float timeInterval = 10.0;
/*******************/

/**** GLOBALS ****/
Rx16Response rx = Rx16Response();

unsigned long bytesReceived = 0;
float throughput = 0;
unsigned long prev = 0;
/****************/

void capture() {
  throughput = bytesReceived / timeInterval / 1000.0;
  bytesReceived = 0;
}

void benchOutput() {
  //Output throughtput in Kb/s
  Serial.println(throughput * 8);
}

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  xbee.setSerial(mySerial);
  pinMode(13, OUTPUT);
}

void loop() {
  xbee.readPacket();

  XBeeResponse &response = xbee.getResponse();
  if(response.isAvailable()) {
    digitalWrite(13, HIGH);
    if(response.getApiId() == RX_16_RESPONSE) {

      if(response.getErrorCode() == NO_ERROR) {
        //subtract 4 because i think it includes an extra 4 bytes
        bytesReceived += xbee.getResponse().getFrameDataLength() - 4;
      }
      
      digitalWrite(13, LOW);
    }
  }

  if(millis() > prev + timeInterval * 1000) {
    capture();
    benchOutput();
    prev = millis();
  }
}
