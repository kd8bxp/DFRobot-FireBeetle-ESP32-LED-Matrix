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
int wait=8; //Between 5 and 10 seem to produce a nice effect.

void setup() {
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
   ht1632c.setCursor(2,1);
 ht1632c.setFont(FONT5X4);
  ht1632c.print("Hello");
   ht1632c.drawLine(0, 0, 0, 7);
  ht1632c.drawLine(0, 7, 23, 7);
  ht1632c.drawLine(23, 7, 23, 0);
  ht1632c.drawLine(0, 0, 23, 0);
  ht1632c.writeScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(uint8_t x=0; x<24; x++){
    //for(uint8_t y=0; y<8; y++){
      ht1632c.setPixel(x,0);
      ht1632c.setPixel(x+2,0);
      ht1632c.setPixel(x+5,0);
      ht1632c.setPixel(23-x,7);
      ht1632c.setPixel(23-x-2,7);
      ht1632c.setPixel(23-x-5,7);
      ht1632c.clrPixel(x+1,0);
      ht1632c.clrPixel(x+3,0);
      ht1632c.clrPixel(x+5,0);
      ht1632c.clrPixel(23-x-1,7);
      ht1632c.clrPixel(23-x-3,7);
      ht1632c.clrPixel(23-x-5,7);   
      ht1632c.writeScreen(); 
      delay(wait);
      //ht1632c.clrPixel(x,0);
      ht1632c.setPixel(x,0);
      ht1632c.setPixel(x+2,0);
      ht1632c.setPixel(x+5,0);
      ht1632c.setPixel(23-x,7);
      ht1632c.setPixel(23-x-2,7);
      ht1632c.setPixel(23-x-5,7);   
      ht1632c.writeScreen(); 
      delay(wait);
    //}
  }

  //for(uint8_t x=0; x<24; x++){
    for(uint8_t y=0; y<8; y++){
      ht1632c.setPixel(23,y);
      ht1632c.setPixel(0,7-y);
      ht1632c.clrPixel(23,y+1);
      ht1632c.clrPixel(0,7-y-1);   
      ht1632c.writeScreen(); 
      delay(wait);
      ht1632c.setPixel(23,y);
      ht1632c.setPixel(0,7-y);
      ht1632c.writeScreen();
      delay(wait);
    //}
  }

 
  
}
