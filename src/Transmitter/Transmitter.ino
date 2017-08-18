#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string>
#include "exponential.h"

#define CONFIG 1

/* DEVICE 1 */
#if CONFIG == 1
char *ssid = "ESPsoftAP_01";
char *pass = "nickkoester";
unsigned int serverPort = 4210;
unsigned int localPort = 2390;
#endif

/* DEVICE 2 */
#if CONFIG == 2
char *ssid = "ESPsoftAP_02";
char *pass = "nickkoester";
unsigned int serverPort = 4220;
unsigned int localPort = 2290;
#endif

IPAddress receiverIP(0, 0, 0, 0);
WiFiUDP Udp;

#define PACKET_SIZE 1460
double arrivalRate = 1.0;
int generatorSeed = 1;

char payload[PACKET_SIZE];
ExponentialDist randomNum(arrivalRate, generatorSeed);

unsigned long sendPacket(IPAddress& address) {
  Udp.beginPacket(address, serverPort);
  Udp.write(payload);
  Udp.endPacket();
}

// attempt to connect to Wifi network:
void connectToServer() {
  //Clear old configuration
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to wifi");
  Serial.println("");
  
  Serial.println("\nStarting connection to server...");
  if (!Udp.begin(localPort)) {
    Serial.println("UDP connection failed");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.print("Using configuration: ");
  Serial.println(CONFIG);
  memset(payload, 'A', PACKET_SIZE);

  pinMode(16, OUTPUT);
  connectToServer();
}

void loop() {
  digitalWrite(16, HIGH);
  sendPacket(receiverIP);
  digitalWrite(16, LOW);
  delay(0);
}
