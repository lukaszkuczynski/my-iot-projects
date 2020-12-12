#include <secrets.h>

/*
 * HTTPS Secured Client GET Request
 * Copyright (c) 2019, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SlackWebhook.h>

/* Set these to your desired credentials. */
const char *ssid = WIFI_SSID; //ENTER YOUR WIFI SETTINGS
const char *password = WIFI_PASSWORD;

//Link to read data from https://jsonplaceholder.typicode.com/comments?postId=7
//Web/Server address to read/write from
const int httpsPort = 443; //HTTPS= 443 and HTTP = 80

//SHA1 finger print of certificate use web browser to view and copy
const char fingerprint[] PROGMEM = "F6 23 3E AC 7A 1D 03 63 15 E2 4F 57 B6 10 23 2E 22 53 51 4E";
const char slack_fingerprint[] PROGMEM = "C1 0D 53 49 D2 3E E5 2B A2 61 D5 9E 6F 99 0D 3D FD 8B B2 B3";

const char *host = "hooks.slack.com";

const String localurl = SLACK_LOCAL_URL;

SlackWebhook slack_webhook = SlackWebhook(host, localurl, slack_fingerprint);

void setup()
{
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA); //Only Station No AP, This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password); //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP
}

void postSlackMessage(String message)
{
  /*--Slack message config--*/
  String slackIcon = "http://icons.iconarchive.com/icons/alecive/flatwoken/128/Apps-Audio-Card-icon.png";
  String slackBotName = "Esp8266";

  String postData = "{\"username\": \"" + slackBotName + "\", \"text\": \"" + message + "\", \"icon_url\": \"" + slackIcon + "\"}";
  slack_webhook.postMessageToSlack(postData);
}
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop()
{
  postSlackMessage("hejbro");

  delay(5000);
}
