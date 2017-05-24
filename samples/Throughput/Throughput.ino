#include <Ticker.h>

double bytes = 0;

int timeInterval = 10;
const int MS = 1000;
const int US = 1000000;

int timestamp = 0;
double throughput = 0;

Ticker tick;
bool printFlag = false;

void output() {  
  Serial.print("Throughput at ");
  Serial.print(timestamp);
  Serial.print(": ");
  Serial.print(throughput);
  Serial.println(" bytes / sec");
}

void capture() {
  timestamp = micros() / US;
  throughput = bytes / timeInterval;
  bytes = 0;
  printFlag = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("");

  tick.attach(timeInterval, capture);
  Serial.println("Starting count...");
}

void loop() {
  delay(random(100, 2000));
  bytes++;

  if(printFlag) {
    output();
    printFlag = false;
  }
}
