#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int timer = 30;


void startmode() {
  strip.setPixelColor(16, strip.Color(0, 255, 0));
  strip.setPixelColor(17, strip.Color(0, 255, 0));
  strip.show();
  delay(200);
  int x = 0;

  for (int i = 0; i <= 16; i++) {
    x = x + 1;

    strip.setPixelColor(16 - x, strip.Color(0, 255, 0));
    strip.setPixelColor(17 + x, strip.Color(0, 255, 0));
    strip.show();
    delay(200);
  }
}

void lysAv() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}



void lightmode1(){
    
    uint16_t i, j;

    
    for (j = 255; j > 200; j--) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, j, 0, 0);
     }
    strip.show();
    delay(20);
    Serial.println(j);
  }
  for (j = 200; j < 255; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
         strip.setPixelColor(i, j, 0, 0);
        }
        strip.show();
        delay(20);
    }
}

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);
  for(int i=0; i<strip.numPixels(); i++) {

        strip.setPixelColor(i,strip.Color(255, 0, 0));
        strip.show();
        delay(timer);
    }
}

void loop(){
    //lightmode1();
    lysAv();
}