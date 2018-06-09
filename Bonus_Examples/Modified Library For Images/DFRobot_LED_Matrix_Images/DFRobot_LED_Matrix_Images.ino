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

byte PROGMEM mouthImg[21] = 
  { 0b11100000, 0b00000000, 0b00000111,
    0b01111111, 0b00000000, 0b11111110,
    0b00111111, 0b11111111, 0b11111100,
    0b00001100, 0b01111110, 0b00110000,
    0b00000000, 0b01111110, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000
    // ...Subsequent bitmaps go here...
  };

uint8_t marioImg [] = { 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x80, 0x0, 0x0, 0x7, 0xf0, 
0x0, 0x0, 0xd, 0xa0, 0x0, 0x0, 0x1c, 0xf0, 0x0, 0x0, 0x1f, 0x90, 0x0, 0x0, 0x7, 
0xe0, 0x0, 0x0, 0xe, 0x40, 0x0, 0x0, 0x1a, 0x70, 0x0, 0x0, 0x19, 0x98, 0x0, 0x0, 
0x1f, 0xf8, 0x0, 0x0, 0x6, 0x60, 0x0, 0x0, 0xe, 0x70, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 
 };

uint8_t IMAGES[] = {
  B00111000,
  B01000100,
  B10010010,
  B10101010,
  B10100010,
  B00100100,
  B00111000,
  B00000000
}; //Sort of a @ sign - my own design really - and it works. Used https://xantorohara.github.io/led-matrix-editor
//to make it.

uint8_t pic1[] = {
B10010101,B01010101,
B00101010,B10101010,
B01000000,B00000010,
B01000000,B00000010,
B01000000,B00000010,
B01000000,B00000010,
B01010101,B01010100,
B10101010,B10101010};

uint8_t mario2Img[] = {
B01011001,B11010000,B00000000,
B01110101,B11110000,B00000000,
B01011111,B01110000,B00000000,
B11111100,B11000000,B00000000,
B11101111,B11000000,B00000000,
B11011110,B01110000,B00000000,
B01111011,B11110000,B00000000,
B00111001,B11010000,B00000000}; //sort of works https://www.riyas.org/2013/12/online-led-matrix-font-generator-with.html
//has a editor that can change sizes, have to edit it's binary values for 8 bits but otherwise seems to work.

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

 ht1632c.drawImage(mouthImg,8,24,0,0,0);
 ht1632c.writeScreen();
 delay(1000);

ht1632c.drawImage(marioImg,16,32,0,0,16);
ht1632c.writeScreen();
delay(1000);

ht1632c.drawImage(IMAGES,8,8,0,0,0);
ht1632c.writeScreen();
delay(1000);

ht1632c.drawImage(pic1,16,8,0,0,0);
ht1632c.writeScreen();
delay(1000);

ht1632c.clearScreen();
ht1632c.drawImage(mario2Img,8,24,0,0,0);
ht1632c.writeScreen();
delay(1000);

ht1632c.clearScreen();

 while(1) {
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
 
 for (int i=0; i<9; i++) {
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

void print_bin(byte x) {
    printf("B");
    for (int i = 0; i < 8; i++) {
        printf("%s", (x >> (7-i)) % 2 ? "1" : "0");
    }
    printf("\n");
}

void reverse(byte* in, byte* out, int width, int height) {
    int width_bytes = (width + 7) / 8;
    int height_bytes = (height + 7) / 8;
    // init *out. You can skip the next line if you are sure that *out is clear.
    //memset (out, 0, width * height_bytes);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (in[(y * width_bytes + x / 8)] & (1 << (7 - x % 8))) {
                out[(x * height_bytes + y / 8)] |= (1 << (7 - y % 8));
            }
        }
    }
}
