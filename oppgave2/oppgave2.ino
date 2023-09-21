#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int potensioMeter1 = A1;
const int potensioMeter2 = A2;
const int potensioMeter3 = A3;

int potensioColor1;
int potensioColor2;
int potensioColor3;

unsigned long lastUpdate = 0;
unsigned long updateInterval = 10; // Adjust this value for the desired update interval in milliseconds

void mode2() {
  int x = 0;

  for (int i = 0; i <= NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(potensioColor1, potensioColor2, potensioColor3));
    strip.show();
  }
  // Viser en oppstartseffekt med grønn farge
}

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);
  pinMode(potensioMeter1, INPUT_PULLUP);
  pinMode(potensioMeter2, INPUT_PULLUP);
  pinMode(potensioMeter3, INPUT_PULLUP);
}

void loop() {
  int meterValue1 = analogRead(potensioMeter1);
  potensioColor1 = map(meterValue1, 0, 1023, 0, 255);

  int meterValue2 = analogRead(potensioMeter2);
  potensioColor2 = map(meterValue2, 0, 1023, 0, 255);

  int meterValue3 = analogRead(potensioMeter3);
  potensioColor3 = map(meterValue3, 0, 1023, 0, 255);

  mode2();
}
