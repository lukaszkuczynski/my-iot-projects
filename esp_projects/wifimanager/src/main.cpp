#include <FS.h> //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <secrets.h>
#include <ESP8266mDNS.h>
#include <TM1637Display.h>

char *accessPointName = ACCESSPOINT_NAME;
char *accessPointPassword = ACCESSPOINT_PASSWORD;

#define RESET_AP_PIN D1
const int CLK = D2; //Set the CLK pin connection to the display
const int DIO = D3;

ESP8266WebServer server(80);

WiFiManager wifiManager;

TM1637Display display(CLK, DIO);

void blinkNtimes(int n)
{
  for (size_t i = 0; i < n; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
}

ICACHE_RAM_ATTR void detectsClick()
{
  Serial.println("click detected! Resetting AP");
  wifiManager.resetSettings();
  blinkNtimes(5);
  ESP.reset();
}

void handleRoot();
void handleNotFound();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RESET_AP_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

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

  if (MDNS.begin("esp8266"))
  { // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  }
  else
  {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

  display.setBrightness(0x0a);
}

void loop()
{
  server.handleClient();
  char s[32];
  int32_t rssi = WiFi.RSSI();
  // String ssid = WiFi.SSID().c_str();
  snprintf(s, sizeof(s), "SSID: zz, %d dbm", rssi);
  Serial.println(s);
  display.showNumberDec(rssi);
  delay(500); // wait for a second
}

void handleRoot()
{ // When URI / is requested, send a web page with a button to toggle the LED
  blinkNtimes(2);
  int32_t rssi = WiFi.RSSI();
  char response[100];
  snprintf(response, sizeof(response), "<h1>hello from ESP</h1><p>signal strength %d </p>", rssi);
  server.send(200, "text/html", response);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}