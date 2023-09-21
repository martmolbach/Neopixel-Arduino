/*#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int potensioMeter1 = A0;

int potensioTimer;

void mode3() {
  for (long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show();
    delay(potensioTimer);
  }
}

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);
  pinMode(potensioMeter1, INPUT_PULLUP);
}

void loop() {
  int meterValue = analogRead(potensioMeter1);
  potensioTimer = map(meterValue, 0, 1023, 1, 100); // Map analog input range to delay range (1-100)
  mode3(); // Call the mode3 function with the updated delay value
}
*/

#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int potensioMeter1 = A0;

int potensioTimer;

unsigned long lastUpdate = 0;
unsigned long updateInterval = 10; // Adjust this value for the desired update interval in milliseconds

void mode3() {
  static long firstPixelHue = 0;  // Static variable to retain its value between function calls
  if (millis() - lastUpdate >= potensioTimer) {
    firstPixelHue += 256;
    strip.rainbow(firstPixelHue);
    strip.show();
    lastUpdate = millis();
  }
}

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);
  pinMode(potensioMeter1, INPUT_PULLUP);
}

void loop() {
  int meterValue = analogRead(potensioMeter1);
  potensioTimer = map(meterValue, 0, 1023, 1, 20); // Map analog input range to delay range (1-100)
  mode3(); // Call the mode3 function with the updated delay value
}
