#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define CONFIG 2

/* DEVICE 1 */
#if CONFIG == 1
char *ssid = "ESPsoftAP_01";
char *pass = "nickkoester";
int channel = 6;
unsigned int localUdpPort = 4210;  // local port to listen on
#endif

/* DEVICE 2 */
#if CONFIG == 2
char *ssid = "ESPsoftAP_02";
char *pass = "nickkoester";
int channel = 1;
unsigned int localUdpPort = 4220;  // local port to listen on
#endif

/** Server **/
WiFiUDP Udp;

/** Benchmarking **/
const int timeInterval = 10;
const int MS = 1000;
const int US = 1000000;

int timestamp = 0;
double packetsReceived = 0;

Ticker tick;
bool printFlag = false;

void benchOutput() {
  Serial.print("Packets received at ");
  Serial.print(timestamp);
  Serial.print(": ");
  Serial.print(packetsReceived);
  Serial.println(" packets");
}

void capture() {
  timestamp = micros() / US;
  printFlag = true;
}

void setupAccessPoint() {
  //Clear old configuration
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  delay(100);

  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP(ssid, pass, channel);
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }

  WiFi.printDiag(Serial);
}

void receiveMessage() {
  int packetSize = Udp.parsePacket();\

  if(packetSize > 0) {
    packetsReceived++;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.print("Using configuration: ");
  Serial.println(CONFIG);

  setupAccessPoint();

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  tick.attach(timeInterval, capture);
  Serial.println("Starting count...");
}

void loop()
{
  receiveMessage();

  if(printFlag) {
    benchOutput();
    printFlag = false;
  }
}
