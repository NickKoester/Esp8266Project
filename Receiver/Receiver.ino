#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define CONFIG 1

/* DEVICE 1 */
#if CONFIG == 1
char *ssid = "ESPsoftAP_01";
char *pass = "nickkoester";
unsigned int localUdpPort = 4210;  // local port to listen on
#endif

/* DEVICE 2 */
#if CONFIG == 2
char *ssid = "ESPsoftAP_02";
char *pass = "nickkoester";
unsigned int localUdpPort = 4220;  // local port to listen on
#endif

/** Server **/
WiFiUDP Udp;
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Received Message";  // a reply string to send back
int prevConnected = 0; //debugging purposes

/** Benchmarking **/
const int timeInterval = 10;
const int MS = 1000;
const int US = 1000000;

int timestamp = 0;
double bytesReceived = 0;
double throughput = 0;

Ticker tick;
bool printFlag = false;

void benchOutput() {
  Serial.print("Throughput at ");
  Serial.print(timestamp);
  Serial.print(": ");
  Serial.print(throughput);
  Serial.println(" bytes / sec");
}

void capture() {
  timestamp = micros() / US;
  throughput = bytesReceived / timeInterval;
  bytesReceived = 0;
  printFlag = true;
}

void setupAccessPoint() {
  //Clear old configuration
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  delay(100);

  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP(ssid, pass);
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

void countConnected() {
  int numConnected = WiFi.softAPgetStationNum();
  if(numConnected != prevConnected) {
      Serial.printf("Stations connected = %d\n", numConnected);
      prevConnected = numConnected;
  }  
}

void receiveMessage() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
  
  bytesReceived += packetSize;
}

void sendAck() {
    // send back a reply to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();  
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
  countConnected();
  receiveMessage();
  //sendAck();

  if(printFlag) {
    benchOutput();
    printFlag = false;
  }
  
  //delay(3000);
}
