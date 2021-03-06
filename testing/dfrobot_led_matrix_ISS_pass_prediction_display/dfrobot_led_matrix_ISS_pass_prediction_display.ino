/*
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
#include <HTTPClient.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <TimeLib.h> 
//#include <DNSServer.h>
//#include <WebServer.h>

long lastMsg = 0;
char msg[50];
int value = 0;


//#if defined( ESP_PLATFORM ) || defined( ARDUINO_ARCH_FIREBEETLE8266 )  //FireBeetle-ESP32 FireBeetle-ESP8266
#define DATA D6
#define CS D2
#define WR D7
//#define RD D8
//#else
//#define DATA 10
//#define CS 25
//#define WR 13
//#define RD 5
//#endif

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);

//Find your Latitude and Longitude here
//https://www.latlong.net/
float mylat = 39.360095;
float mylon = -84.58558;
float isslat, isslon;
int distance, number, count;
String payload;
String name[10], craft[10],risetime[5];
float duration[5];
 
const String iss = "http://api.open-notify.org/iss-now.json"; 
const String ppl = "http://api.open-notify.org/astros.json";
String pas = "http://api.open-notify.org/iss-pass.json?";

void setup() {
  Serial.begin(9600);
  
 display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setCursor(0,0);
 display.setFont(FONT8X4);
 display.print("ISS Notification Display!", 30);
  smartConnect();
  pas = pas + "lat=" + (String)mylat+"&lon="+ (String)mylon;
}

void loop() {
  getJson(iss);
  //Serial.println(payload);   //Print the response payload
 decodeLocJson();
 getDistance();
 issLocOLEDDisplay();
 issLocSerialDisplay();
 //delay(500);
 
  
  getJson(pas);
  decodePassJson();
  displayPassSerial();
  displayPassOLED();
  //delay(500);
  
 getJson(ppl);
 decodePeopleJson();
 displayPeopleSerial();
 displayPeopleOLED();
 //delay(500);    //Send a request every 30 seconds
 
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

void issLocOLEDDisplay() {
  display.clearScreen();
  display.print("The ISS is currently at: ", 30);
 char temp[15];
 sprintf(temp, " %d.%02d,%d.%02d ",(int)isslat,abs((int)(isslat*100)%100),(int)isslon,abs((int)(isslon*100)%100));
 display.print(temp, 30);
 char temp1[30];
 sprintf(temp1, "ISS is about %d miles", distance);
 display.print(temp1,30);
 display.print(" from you.",30);
 display.print(" And moving fast!!", 30);
 
}

void issLocSerialDisplay() {
  Serial.print("The ISS is currently at ");
  Serial.print(isslat, 4); Serial.print(","); Serial.println(isslon,4);
  Serial.print("The ISS is about "); Serial.print(distance); Serial.println(" miles from you now.\nAnd moving fast!");
    
}

void getJson(String url) {
  
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     HTTPClient http;  //Declare an object of class HTTPClient
     http.begin(url);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
     if (httpCode > 0) { //Check the returning code
       payload = http.getString();   //Get the request response payload
     
    }
 
    http.end();   //Close connection
 
  }
}

void decodeLocJson() {
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  isslat=root["iss_position"]["latitude"];
  isslon=root["iss_position"]["longitude"];
}

void getDistance() {
  float theta, dist, miles;
  theta = mylon - isslon;
  dist = sin(deg2rad(mylat)) * sin(deg2rad(isslat)) + cos(deg2rad(mylat)) * cos(deg2rad(isslat)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  miles = dist * 60 * 1.1515;
  distance = miles;
}

float deg2rad(float n) {
  float radian = (n * 71)/4068;
  return radian;
}

float rad2deg(float n) {
  float degree = (n*4068)/71;
  return degree;
}

void decodePeopleJson() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  number = root["number"];
  if (number > 10) {number = 10;}
  for (int i=0;i<number; i++){
    
    const char* temp1 = root["people"][i]["name"];
    const char* temp2 = root["people"][i]["craft"];
    name[i] = (String)temp1;
    craft[i] = (String)temp2;
  }
 }

 void displayPeopleSerial() {
  Serial.print("There are "); Serial.print(number); Serial.println(" people in space right now.");
  for (int i=0;i<number; i++) {
    Serial.print(name[i]);Serial.print(" on board ");Serial.println(craft[i]);
  }
 }

 void displayPeopleOLED() {
  display.clearScreen();
  char temp2[50];
  sprintf(temp2, "%d people are in space. ", number);
  display.print(temp2,30);

 if (number > 5) {number = 5;} //Display the 1st 5 Astros on OLED 
 for (int i=0;i<number; i++) {
  char temp2[100];
  sprintf(temp2,"%s, %s ", name[i],craft[i]);
 //display.print(name[i] + ", " + craft[i]+" ",30);
 display.print(temp2);
 }
}

void decodePassJson() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  count = root["request"]["passes"];
  
  if (count > 5) {count = 5;}
  for (int i=0;i<count; i++){
    
    unsigned int tempEpoch = root["response"][i]["risetime"];
    risetime[i] = convertEpoch(tempEpoch);
    duration[i] = root["response"][i]["duration"];
    duration[i] = duration[i] / 60;
      }
 }

 String convertEpoch(unsigned int epoch) {
  int h = hour(epoch);
  int m = minute(epoch);
  int d = day(epoch);
  int mn = month(epoch);
  int y = year(epoch);
   char temp[100];
  sprintf(temp, "RT: %d/%d %d:%d UTC",mn,d,h,m);
  return (String)temp;
 }

void displayPassSerial() {
  Serial.println("Pass Prediction");
  for (int i=0;i<count; i++) {
    Serial.print(risetime[i]); Serial.print(" [");Serial.print(duration[i]);Serial.println(" mins.]");
  }
}

 void displayPassOLED() {
  display.clearScreen();
  display.print("Pass Prediction ",30);

 for (int i=0;i<count; i++) {
   char temp[100];
  sprintf(temp, "%d [ %d m]",risetime[i],duration[i]);
 //display.print((String)risetime[i] + " [" + (String)duration[i]+" m] ",30);
 display.print(temp,30);
 }

}

