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

uint8_t i1[16] = {0,0,1,3,0,0,0,0,0,0,0,0,0,0,3,1};
uint8_t i2[16] = {0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0};
uint8_t i3[16] = {0,0,0,0,0,4,0,0,0,0,0,4,0,0,0,0};
uint8_t i4[16] = {0,0,0,0,0,0,8,0,0,8,0,0,0,0,0,0};
uint8_t i5[16] = {0,0,0,0,0,0,16,40,16,0,0,0,0,0,0,0};
uint8_t i6[16] = {0,0,0,0,0,16,48,44,80,16,0,0,0,0,0,0};
uint8_t i7[16] = {0,0,0,0,8,80,32,6,64,80,8,0,0,0,0,0};
uint8_t i8[16] = {0,0,0,0,76,64,0,3,0,64,40,4,0,0,0,0};
uint8_t i9[16] = {0,0,0,32,70,0,0,1,0,0,48,6,0,0,0,0};
uint8_t i10[16] = {0,0,0,48,3,0,0,0,0,0,24,3,0,0,0,0};
uint8_t i11[16] = {0,0,0,8,1,0,0,0,0,0,4,1,0,0,0,0};
uint8_t i12[16] = {0,0,0,4,0,0,0,0,0,0,2,0,0,0,0,0};
uint8_t i13[16] = {0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t i14[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int wait = 700; //1000

void setup() {
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
}

void loop() {
  ht1632c.clearScreen();
 
 for (int i=0; i<16; i++) {
  fireworks(i); 
 }
 delay(wait*0.2);
} 
  
void fireworks(int x) {  
  ht1632c.drawImage(i1,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i2,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i3,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i4,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i5,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i6,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i7,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i8,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i9,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i10,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i11,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i12,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i13,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);
  ht1632c.drawImage(i14,8,8,x,0,0);
  ht1632c.writeScreen();
  delay(wait*0.2);

}

