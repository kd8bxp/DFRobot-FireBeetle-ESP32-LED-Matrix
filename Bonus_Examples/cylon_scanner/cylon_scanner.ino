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

int wait=15;

void setup() {
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
  
}

void loop() {
for (uint8_t x=0; x< 12; x++) {
  ht1632c.setPixel(x,3);
  ht1632c.setPixel(x,4);
  ht1632c.setPixel(24-x,3);
  ht1632c.setPixel(24-x,4);
  ht1632c.writeScreen();
  delay(wait);
  ht1632c.clrPixel(x,3);
  ht1632c.clrPixel(x,4);
  ht1632c.clrPixel(24-x,3);
  ht1632c.clrPixel(24-x,4);
  ht1632c.writeScreen();
  delay(wait);
}

for (uint8_t x=12; x>1; x--) {
  ht1632c.setPixel(x,3);
  ht1632c.setPixel(x,4);
  ht1632c.setPixel(24-x,3);
  ht1632c.setPixel(24-x,4);
  ht1632c.writeScreen();
  delay(wait);
  ht1632c.clrPixel(x,3);
  ht1632c.clrPixel(x,4);
  ht1632c.clrPixel(24-x,3);
  ht1632c.clrPixel(24-x,4);
  ht1632c.writeScreen();
  delay(wait);
}
  
}
