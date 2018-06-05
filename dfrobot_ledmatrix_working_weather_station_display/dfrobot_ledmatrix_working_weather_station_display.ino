/*
 * Displays current WX Condition and Tempature.
 * based on the ESP8266 Weather Station code by
 * Squix78 (version 1.5.0 of the library appears working
 * on the ESP32 FireBeetle.
 * 
 * Version 1.0.0 - June 2, 2018
 * Known Issues: SmartConfig doesn't work as expected.
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

#include "DFRobot_HT1632C.h"
#include <WiFi.h>
#include "WundergroundClient.h" //https://github.com/ThingPulse/esp8266-weather-station  Need at least version 1.5.0 of library
#include <JsonListener.h>

#define DATA D6
#define CS D2
#define WR D7

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);

const int UPDATE_INTERVAL = 1 * 60000; // Update every 10 minutes
static int taskCore = 0;

// Wunderground Settings
const boolean IS_METRIC = false;
const boolean usePM = true;
const String WUNDERGRROUND_API_KEY = "";
const String WUNDERGRROUND_LANGUAGE = "EN";
const String WUNDERGROUND_COUNTRY = "OH";
const String WUNDERGROUND_CITY = "Dayton";

// Set to false, if you prefere imperial/inches, Fahrenheit
WundergroundClient wunderground(IS_METRIC);

void coreTask( void * pvParameters ){

 while(true) {
    delay(UPDATE_INTERVAL);
    updateData();
   }
 yield();
}


void setup() {
   Serial.begin(9600);
   
 display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setCursor(0,0);
 display.setFont(FONT8X4);
 display.print("Weather Display.", 30);
 smartConnect();
 display.print("Updating...",30);
updateData();
xTaskCreatePinnedToCore(
                    coreTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
 
 
}

void loop() {

CurrentWeather();
delay(150);
yield();

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

void CurrentWeather() {
  display.clearScreen();
  display.setCursor(0,0);
  String wxTXT = "Today: " + wunderground.getWeatherText();
  const char *temp;
  temp = wxTXT.c_str();
  display.print(temp,30);
  //display->drawString(60 + x, 5 + y, wunderground.getWeatherText());
    String tempature = "Current Tempature: " + wunderground.getCurrentTemp() + "°F";
    const char *temp1;
    temp1 = tempature.c_str();
  //display->drawString(60 + x, 15 + y, temp);
  display.print(temp1,30);
}

void updateData() {
   wunderground.updateConditions(WUNDERGRROUND_API_KEY, WUNDERGRROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
    }


