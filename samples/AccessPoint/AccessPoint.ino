#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();

  //Clear old configuration
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  delay(100);

  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP("ESPsoftAP_01", "nickkoester");
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

void loop()
{
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(3000);
}
