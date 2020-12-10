#include <FS.h> //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <secrets.h>

char *accessPointName = ACCESSPOINT_NAME;
char *accessPointPassword = ACCESSPOINT_PASSWORD;

#define RESET_AP_PIN D1

bool resetAp = false;
WiFiManager wifiManager;

ICACHE_RAM_ATTR void detectsClick()
{
  Serial.println("click detected! Resetting AP");
  wifiManager.resetSettings();
  ESP.reset();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RESET_AP_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RESET_AP_PIN), detectsClick, RISING);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect(accessPointName, accessPointPassword))
  {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

void loop()
{
  char s[32];
  int32_t rssi = WiFi.RSSI();
  // String ssid = WiFi.SSID().c_str();
  snprintf(s, sizeof(s), "SSID: zz, %d dbm", rssi);
  Serial.println(s);
  delay(1000); // wait for a second
}
