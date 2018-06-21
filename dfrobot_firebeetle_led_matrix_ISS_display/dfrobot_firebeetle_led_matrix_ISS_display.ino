/*
 * ISS Pass Prediction and Location Display
 * for FireBeetle ESP32 and LED Matrix
 * Using 2nd core for more control.
 * 
 * version 2.0.0 - June 2, 2018 
 * Known Issues: SmartConfig doesn't work as Expected.
 * Sketch may display more information that needed.
 * 
 * version 2.1.0 - June 6, 2018 
 * SmartConfig replaced with WiFiManager
 * added a configuration website so you can decide on the fly
 * what you want displayed. 
 * 
 * See below for more on how to setup for your location
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

int locDis = 1; //Display Location of ISS
int pasDis = 0; //Display Pass Predictions
int pplDis = 1; //Display People in Space

WiFiServer server(80);

#define staticIP 192,168,0,200
#define gateway 192,168,0,1
#define subnet 255,255,255,0

// Client variables 
char linebuf[80];
int charcount=0;

#include "DFRobot_HT1632C.h"
#include <HTTPClient.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <TimeLib.h> 

#define DATA D6
#define CS D2
#define WR D7

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
static int taskCore = 0;

const long interval = 15 * 60000; // Update every 15 minutes
 
const String iss = "http://api.open-notify.org/iss-now.json"; 
const String ppl = "http://api.open-notify.org/astros.json";
String pas = "http://api.open-notify.org/iss-pass.json?";

void coreTask( void * pvParameters ){

 while(true) {
   //WiFiClient client = server.available();
  //if (client) { webConfig(); }
   webConfig();
   yield();
    //delay(interval);
    //getJson(pas);
    //decodePassJson();
    //getJson(ppl);
    //decodePeopleJson();
   }
 yield();
}

void updateISSTask ( void * pvParameters ) {
  while(true) {
    delay(interval);
    getJson(pas);
    decodePassJson();
    getJson(ppl);
    decodePeopleJson();
    Serial.println("updateISSTask ran");
  }
  yield();
}

void setup() {
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  xTaskCreatePinnedToCore(
                    coreTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
  xTaskCreate( updateISSTask,"updateISSTake",10000,NULL,1,NULL);
  display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setCursor(0,0);
  display.setFont(FONT8X4);
  display.print("ISS Notification Display!", 30);
  WiFiManager wifiManager;
  //wifiManager.setSTAStaticIPConfig(IPAddress(staticIP),IPAddress(gateway),IPAddress(subnet));
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.print("Connected.....",30);
  pas = pas + "lat=" + (String)mylat+"&lon="+ (String)mylon;
  getJson(pas);
  decodePassJson();
  getJson(ppl);
  decodePeopleJson();
  
  server.begin();
}

void loop() {
 
  
  if (locDis == 1) {
  getJson(iss);
  decodeLocJson();
  getDistance();
  issLocLEDDisplay();
  }

  if (pasDis == 1) {
  displayPassLED();
  }
  if (pplDis == 1) {
    displayPeopleLED();
  }
  yield();
  delay(100);

}

void webConfig() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>ISS Display Configuration</h1>");
          client.print("<p>Display Location: ");
          client.print(locDis ? "Yes" : "No");
          client.print(" Pass Predict: ");
          client.print(pasDis ? "Yes" : "No");
          client.print(" People: ");
          client.println(pplDis ? "Yes" : "No");
          client.println("<br><br>Display: ");
          client.println("<p>ISS Location: <a href=\"loc1\"><button>YES</button></a>&nbsp;<a href=\"loc0\"><button>NO</button></a></p>");
          client.println("<p>Pass Prediction: <a href=\"pass1\"><button>YES</button></a>&nbsp;<a href=\"pass0\"><button>NO</button></a></p>");
          client.println("<p>People: <a href=\"ppl1\"><button>YES</button></a>&nbsp;<a href=\"ppl0\"><button>NO</button></a></p>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /loc1") > 0){
            //Serial.println("LED 1 ON");
            locDis = 1;
          }
          else if (strstr(linebuf,"GET /loc0") > 0){
            //Serial.println("LED 1 OFF");
            locDis = 0;
          }
          else if (strstr(linebuf,"GET /pass1") > 0){
            //Serial.println("LED 2 ON");
            pasDis = 1;
          }
          else if (strstr(linebuf,"GET /pass0") > 0){
            pasDis = 0;
          }
          else if (strstr(linebuf,"GET /ppl1") > 0) {
          pplDis = 1;
        }
        else if (strstr(linebuf,"GET /ppl0") >0) {
        pplDis = 0;
      }
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
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
  sprintf(temp, "RiseTime: %d/%d %d:%d UTC",mn,d,h,m);
  return (String)temp;
 }

void displayPassLED() {
  
  display.setCursor(0,0);
  display.print("Pass Predictions: ",30);

  for (int i=0;i<count; i++) {
  const char *temp1;
  temp1=risetime[i].c_str();

  display.setCursor(0,0);
  display.print(temp1,30);
  display.setCursor(0,0);
  char temp[50];
  sprintf(temp, "Duration: [%d.%02d mins]",(int)duration[i],abs((int)(duration[i]*100)%100));
  display.print(temp,30);
   }
}

void issLocLEDDisplay() {
  
 char temp[200];
 sprintf(temp, "The ISS is near: %d.%02d,%d.%02d ",(int)isslat,abs((int)(isslat*100)%100),(int)isslon,abs((int)(isslon*100)%100));
 display.setCursor(0,0);
 display.print(temp, 30);
 char temp1[125];
 sprintf(temp1, "About %d miles from you.", distance);
 display.setCursor(0,0);
 display.print(temp1,30);
 display.setCursor(0,0);
 display.print("And moving away fast!!",30);
  
}

void displayPeopleLED() {
  
  char temp2[50];
  sprintf(temp2, "There are %d people in space. ", number);
  display.setCursor(0,0);
  display.print(temp2,30);

  //if (number > 5) {number = 5;} //Display the 1st 5 Astros on LED 
   for (int i=0;i<number; i++) {
    String rTemp = name[i] + " on board " + craft[i];
    const char *temp1;
  temp1 = rTemp.c_str();
  display.setCursor(0,0);
  display.print(temp1,30);
 
  }
}


