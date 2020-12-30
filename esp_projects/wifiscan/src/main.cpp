#include "WiFi.h"
#include "secrets.h"

void wificonnect()
{
  uint8_t wifi_retry = 0; // COUNTER SOLVES ESP32-BUG WITH CERTAIN ROUTERS: CONNECTION ONLY ESTABLISHED EVERY SECOND TIME

  while (WiFi.waitForConnectResult() != WL_CONNECTED && wifi_retry < 3)
  {

    Serial.print("Not yet connected...retrying");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1000);
    wifi_retry++;
  }

  if (wifi_retry >= 3)
  {

    Serial.println("no connection, restarting");
    ESP.restart();
  }

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {

    Serial.print("connected as: ");
    Serial.println(WiFi.localIP());
  }
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  wificonnect();
}

void loop()
{
  Serial.println(WiFi.localIP());
  delay(3000);

  // Serial.println("scan start");

  // // WiFi.scanNetworks will return the number of networks found
  // int n = WiFi.scanNetworks();
  // Serial.println("scan done");
  // if (n == 0)
  // {
  //   Serial.println("no networks found");
  // }
  // else
  // {
  //   Serial.print(n);
  //   Serial.println(" networks found");
  //   for (int i = 0; i < n; ++i)
  //   {
  //     // Print SSID and RSSI for each network found
  //     Serial.print(i + 1);
  //     Serial.print(": ");
  //     Serial.print(WiFi.SSID(i));
  //     Serial.print(" (");
  //     Serial.print(WiFi.RSSI(i));
  //     Serial.print(")");
  //     Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
  //     delay(10);
  //   }
  // }
}