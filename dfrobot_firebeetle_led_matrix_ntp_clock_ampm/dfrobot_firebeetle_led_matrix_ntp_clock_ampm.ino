/*
 * FireBeetle LED MATRIX NTP Clock
 * 
 * Sketch will display time in either 24 hour format (Also know as Military Time)
 * or with AM or PM.  See below for more on how to setup.
 * 
 * version 1.0.0 - June 1, 2018 
 * 
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/

#include <NTPClient.h>  //https://github.com/arduino-libraries/NTPClient
#include "DFRobot_HT1632C.h"
#include <WiFi.h>
#include <WiFiUdp.h>

#define DATA D6
#define CS D2
#define WR D7

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#define TIMEOFFSET -14400 //Find your Time Zone off set Here https://www.epochconverter.com/timezones OFF Set in Seconds
#define AMPM 1 //1 = AM PM time, 0 = MILITARY/24 HR Time

char temp1[30];

void setup() {
  Serial.begin(9600);
 display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setCursor(0,0);
 display.setFont(FONT8X4);
 //display.print("Clock.", 30);
  smartConnect();
  timeClient.begin();
  timeClient.setTimeOffset(TIMEOFFSET);
}

void loop() {
  timeClient.update();
Serial.println(timeClient.getFormattedTime());
//String temp = timeClient.getFormattedTime();
//const char *temp1;
//temp1 = temp.c_str();
if (AMPM) {
  //char temp1[30];
  if (timeClient.getHours() > 13) {
    sprintf(temp1,"%d:%d:%d pm",timeClient.getHours() - 12,timeClient.getMinutes(),timeClient.getSeconds());
  } else { 
    if (timeClient.getHours() == 0) {
      sprintf(temp1,"12:%d:%d am",timeClient.getMinutes(),timeClient.getSeconds());
    } else {
    sprintf(temp1,"%d:%d:%d am",timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds());
  }
 }
} else {
  //char temp1[30];
  sprintf(temp1,"%d:%d:%d",timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds());
}
display.print(temp1,20);
delay(150);
}

void smartConnect() {
  //Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();

  //Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");
  display.print("Waiting...",20);
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
    display.print("Waiting...",20);
  }

  Serial.println("");
  Serial.println("SmartConfig received.");
display.print("SmartConfig received.", 20);
  //Wait for WiFi to connect to AP
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected.");
  display.print("Connected.", 20);
  Serial.print("IP Address: ");
  //display.print(WiFi.localIP(),50);
  Serial.println(WiFi.localIP());
}

