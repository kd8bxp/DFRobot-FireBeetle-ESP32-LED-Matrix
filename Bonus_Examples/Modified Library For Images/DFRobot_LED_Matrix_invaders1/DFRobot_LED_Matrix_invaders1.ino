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

#if defined( ESP_PLATFORM ) || defined( ARDUINO_ARCH_FIREBEETLE8266 )  //FireBeetle-ESP32 FireBeetle-ESP8266
#define DATA D6
#define CS D2
#define WR D7
//#define RD D8
#else
#define DATA 6
#define CS 2
#define WR 7
//#define RD 8
#endif

DFRobot_HT1632C ht1632c = DFRobot_HT1632C(DATA, WR,CS);


//https://www.riyas.org/2013/12/online-led-matrix-font-generator-with.html
uint8_t invader1[] = {B01100010,
B11111111,
B10011001,
B10011001,
B11111111,
B01100110,
B01100110,
B00000000};

uint8_t invader2[] = {B01100010,
B11111111,
B10011001,
B10011001,
B11111111,
B01100110,
B11000011,
B00000000};


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
}

void loop() {
  ht1632c.clearScreen();
 
ht1632c.drawImage(invader1,8,8,0,0,0);
ht1632c.drawImage(invader2,8,8,9,0,0);
ht1632c.drawImage(invader1,8,8,17,0,0);
ht1632c.writeScreen();
delay(500);
ht1632c.drawImage(invader2,8,8,0,0,0);
ht1632c.drawImage(invader1,8,8,9,0,0);
ht1632c.drawImage(invader2,8,8,17,0,0);
ht1632c.writeScreen();
delay(500);  
 }
 
