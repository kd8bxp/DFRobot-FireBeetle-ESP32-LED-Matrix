/*
 * Displays current WX Condition and Tempature.
 * based on the ESP8266 Weather Station code by
 * Squix78 (version 1.5.0 of the library appears working
 * on the ESP32 FireBeetle.
 * 
 * Version 1.0.0 - June 2, 2018
 * Known Issues: SmartConfig doesn't work as expected.
 * 
 * version 1.0.1 - June 6, 2018
 * Replaced SmarConfig with WiFiManager
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
#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          
#endif

//needed for library
#include <DNSServer.h>  //https://github.com/bbx10/DNSServer_tng
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h> //https://github.com/bbx10/WebServer_tng
#endif
#include <WiFiManager.h>         //https://github.com/bbx10/WiFiManager/tree/esp32

#include "DFRobot_HT1632C.h"
//#include "WundergroundClient.h" //https://github.com/ThingPulse/esp8266-weather-station  Need at least version 1.5.0 of library
#include "OpenWeatherMapCurrent.h" //Wunderground has been deprecated, OpenWeatherMap is included in the above library
//#include "OpenWeatherMapForecast.h" 
#include <JsonListener.h> //https://github.com/squix78/json-streaming-parser

#define DATA D6
#define CS D2
#define WR D7

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);

const int UPDATE_INTERVAL = 10 * 60000; // Update every 10 minutes
static int taskCore = 0;

// Wunderground Settings
const boolean IS_METRIC = false;
//const boolean usePM = true;
/*const String WUNDERGRROUND_API_KEY = "";
const String WUNDERGRROUND_LANGUAGE = "EN";
const String WUNDERGROUND_COUNTRY = "OH";
const String WUNDERGROUND_CITY = "Dayton";
*/

//API Key goes here see: https://docs.thingpulse.com/how-tos/openweathermap-key/
//for information on how to obtain a key.
String OPEN_WEATHER_MAP_APP_ID = ""; // API KEY HERE!!!
String OPEN_WEATHER_MAP_LOCATION = "Dayton, US"; //Check OpenWeatherMap for the name of your town or location
String OPEN_WEATHER_MAP_LANGUAGE = "en";
//const uint8_t MAX_FORECASTS = 4;

// Set to false, if you prefere imperial/inches, Fahrenheit
//WundergroundClient wunderground(IS_METRIC);

OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapCurrent currentWeatherClient;

//OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
//OpenWeatherMapForecast forecastClient;

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
 WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  Serial.println("WiFi connected");
  display.print("Connected.....",30);
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

void CurrentWeather() {
  display.clearScreen();
  display.setCursor(0,0);
  String wxTXT = "Today: " + currentWeather.description; //wunderground.getWeatherText();
  Serial.println(wxTXT);
  const char *temp;
  temp = wxTXT.c_str();
  Serial.println(temp);
  display.print(temp,45);
  
  String temp3 = String(currentWeather.temp, 1) + (IS_METRIC ? " C" : " F");
  Serial.println(temp3);
    String tempature = "Current Tempature: " + temp3;
    const char *temp1;
    temp1 = tempature.c_str();
  Serial.println(temp1);
  display.print(temp1,45);
}

void updateData() {
   //wunderground.updateConditions(WUNDERGRROUND_API_KEY, WUNDERGRROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrent(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION);
  //forecastClient.setMetric(IS_METRIC);
  //forecastClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  //uint8_t allowedHours[] = {12};
  //forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
  //forecastClient.updateForecasts(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION, MAX_FORECASTS);

    }


