/*
 * Very simple Moving Message Display using
 * DFRobot FireBeetle ESP32 and LED Matrix
 * 
 * This sketch listens to a MQTT topic, and displays
 * what it hears on the LED Matrix.
 * 
 * version 1.0.0 - June 2, 2018
 * 
 * Known issues:
 * SmartConfig doesn't work on the ESP32 as expected.
 * Message is limited to between 24 to 27 characters
 * (limitation of the DFRobot Library(??) or maybe memory(?))
 * 
 * version 1.0.1 - June 6, 2018
 * Replaced SmarConfig with WifiManager
 * 
 * See below for more on how to setup.
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
#include <PubSubClient.h>

#define DATA D6
#define CS D2
#define WR D7

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);

const char* mqtt_server = "test.mosquitto.org"; //MQTT broker
//const char* mqtt_server = "rpi-speak.local";
const char* subTopic = "matrixDisplay10"; //subscribe topic

WiFiClient espClient;
PubSubClient client(espClient);

String msg;
int value = 0;

void setup() {
  Serial.begin(9600);
   
 display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setCursor(0,0);
 display.setFont(FONT8X4);
 display.print("MQTT Message Display.", 30);
 WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  Serial.println("WiFi connected");
  display.print("Connected.....",30);
 client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
   reconnect();
}

void loop() {
client.loop();
const char *temp;
temp = msg.c_str();
display.print(temp,30);
delay(150);
yield();  

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
    msg = "";
  for (int i = 0; i < length; i++) {
     Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    uint64_t chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
    char clientid[25];
    snprintf(clientid,25,"WIFI-Display-%08X",chipid); //this adds the mac address to the client for a unique id
    Serial.print("Client ID: ");
    Serial.println(clientid);
    if (client.connect(clientid)) {
      Serial.println("connected");
      client.subscribe(subTopic);
      //scroll("Connected...");
      //scroll(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
