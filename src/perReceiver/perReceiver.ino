#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define CONFIG 2

/* DEVICE 1 */
#if CONFIG == 1
char *ssid = "ESPsoftAP_01";
char *pass = "nickkoester";
int channel = 11;
float dBm = 20.5;
WiFiPhyMode_t phy = WIFI_PHY_MODE_11B;
unsigned int localUdpPort = 4210;  // local port to listen on
#endif

/* DEVICE 2 */
#if CONFIG == 2
char *ssid = "ESPsoftAP_02";
char *pass = "nickkoester";
int channel = 11;
float dBm = 5.0;
WiFiPhyMode_t phy = WIFI_PHY_MODE_11B;
unsigned int localUdpPort = 4220;  // local port to listen on
#endif

#define PACKET_SIZE 1112
/** Server **/
WiFiUDP Udp;

/** Benchmarking **/
const int timeInterval = 10;
const int MS = 1000;
const int US = 1000000;

int timestamp = 0;
double packetsReceived = 0;
char checkMessage[PACKET_SIZE];
char recvPacket[PACKET_SIZE];

Ticker tick;
bool printFlag = false;

void benchOutput() {
  Serial.println(packetsReceived);
}

void capture() {
  //timestamp = micros() / US;
  printFlag = true;
}

void setupAccessPoint() {
  //Clear old configuration
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.setOutputPower(dBm);
  WiFi.setPhyMode(phy);
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
  digitalWrite(16, LOW);
  int packetSize = Udp.parsePacket();
  if(packetSize > 0) {
    int len = Udp.read(recvPacket, PACKET_SIZE);
    if(len > 0) {
      if(!memcmp(checkMessage, recvPacket, PACKET_SIZE)) {
        packetsReceived++;
      }
    }
    digitalWrite(16, HIGH);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.print("Using configuration: ");
  Serial.println(CONFIG);

  pinMode(16, OUTPUT);
  
  setupAccessPoint();
  memset(checkMessage, 'A', PACKET_SIZE);

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
