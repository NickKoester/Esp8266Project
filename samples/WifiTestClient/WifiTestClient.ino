#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "BridgeTest";
const char* password = "nickkoester";

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}


void loop()
{
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
  } else {
    Serial.println("Not connected");
  }
  delay(1000);
}
