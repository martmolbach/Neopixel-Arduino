#include <Adafruit_NeoPixel.h>


					// HUSK //
/* 	 Lyssensoren er på det laveste når den er på default.
   Skru lyssensoren på maks, før du holder inne startknappen	*/




//Definerer pin-nummeret og antall LED-er i NeoPixel-stripen
#define PIN 13
#define NUMPIXELS 32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Definerer en konstant som skal bli brukt i delay senere.
const int timer = 100;

// Definerer pin-nummer for knapper og sensor.
const int powerButton = A1;
const int colorButton = A2;
const int modeButton = A3;
const int lightSensor = A4; 

// Initialiserer en variabel som holder styr på om programmet har startet eller ikke
bool programStarted = false;

//Variabler for knappens tilstand og om knappen er blitt trykket
int colorButtonState = 0;
bool colorButtonPressed = false;

//Variabler for aktiv modus og om modusknappen er blitt trykket
int activeMode = -1;
bool modeButtonPressed = false;

//Variabel som holder styr på nåværende farge, og variabel som lagrer verdien til lyssensoren
int currentColor = 0; 
int lightValue; 


//Funksjon for å sjekke modusknappen
void CheckmodeButton() {

  int buttonState = digitalRead(modeButton);
  
  //Hvis knappen er trykket og den ikke allerede var trykket
  if (buttonState == LOW && !modeButtonPressed) {
    delay(50);

    activeMode++;
    if (activeMode > 2) {
      activeMode = 0;
    }

    modeButtonPressed = true;
  } else if (buttonState == HIGH) {
    modeButtonPressed = false;
  }
  //Oppdaterer aktiv modus basert på knappetrykk
}

//Funksjon for å endre fargen
void ChangeColor() {
  currentColor++;
  if (currentColor > 2) {
    currentColor = 0;
  }
  Serial.print("Color changed to: ");
  if (currentColor == 0) {
    Serial.println("Red");
  } else if (currentColor == 1) {
    Serial.println("Green");
  } else if (currentColor == 2) {
    Serial.println("Blue");
  }
  //Endrer nåværende farge og skriver ut nye fargen i SM
}

// Funksjon for å sette fargen på LED-ene i stripen
void ApplyColor(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
  //Oppdaterer fargen på LED-ene (Denne brukes mye fremover).
}

//Funksjon for å slå av LED-ene
void lysAv() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

//Funksjon for å vise en oppstartseffekt
void startmode() {
  strip.setPixelColor(4, strip.Color(0, 255, 0));
  strip.setPixelColor(5, strip.Color(0, 255, 0));
  strip.show();
  delay(200);
  int x = 0;

  for (int i = 0; i <= 4; i++) {
    x = x + 1;

    strip.setPixelColor(4 - x, strip.Color(0, 255, 0));
    strip.setPixelColor(5 + x, strip.Color(0, 255, 0));
    strip.show();
    delay(200);
    //Denne funksjonen har egne delays for å åpna riktig resultat.
    //Her valgte jeg å ikke bruke timer variablen min.
  }
  delay(400);
  //Viser en oppstartseffekt med grønn farge
}

//Oppsett-funksjon som kjører én gang ved oppstart
void setup() {
  strip.begin();
  strip.show();
  //Starter NeoPixel-stripen
  pinMode(powerButton, INPUT_PULLUP);
  pinMode(colorButton, INPUT_PULLUP);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(lightSensor, INPUT); 
  //Setter opp pin-mode for knapper og lyssensor
  Serial.begin(9600);
  //Starter seriell kommunikasjon
}

//Funksjon for å sjekke om knappen er blitt holdt nede i en viss tid
bool isButtonPressedFor(unsigned long duration) {
  unsigned long startTime = millis();
  while (digitalRead(powerButton) == HIGH) {
    if (millis() - startTime >= duration) {
      return true;
    }
  }
  return false;
}

//Funksjon for modus 1 (Static)
void mode1() {
  ApplyColor(currentColor == 0 ? 255 : 0, currentColor == 1 ? 255 : 0, currentColor == 2 ? 255 : 0);
  delay(timer);
  Serial.print("Mode: 1, ");
  Serial.print("Color: ");
  if (currentColor == 0) {
    Serial.println("Red");
  } else if (currentColor == 1) {
    Serial.println("Green");
  } else if (currentColor == 2) {
    Serial.println("Blue");
  }
  //Setter fargen basert på nåværende farge.
  //Serieprinter Mode og nåværende farge
}

//Variabler som holder styr på hvilken del av blinkingen funksjonen er på.
int mode2Step = 0;
int mode3Step = 0;

//Funksjon for modus 2 (Blinking)
void mode2() {
  switch (mode2Step) {
    case 0:
      //Hvis nåværende farge er 1, 2 eller 3. Sett fargen til r,b eller g. Hvis ikke, 0.
      ApplyColor(currentColor == 0 ? 255 : 0, currentColor == 1 ? 255 : 0, currentColor == 2 ? 255 : 0); 
      delay(timer);
      mode2Step = 1;
      break;
    case 1:
      //Setter fargen til 0, altså av.
      ApplyColor(0, 0, 0);
      delay(timer);
      mode2Step = 0;
      Serial.print("Mode: 2, ");
      Serial.print("Color: ");
      if (currentColor == 0) {
        Serial.println("Red");
      } else if (currentColor == 1) {
        Serial.println("Green");
      } else if (currentColor == 2) {
        Serial.println("Blue");
      }
      break;
  }
  //Skriver ut modus og farge
}

// Funksjon for modus 3 (Regnbue modus)
void mode3() {
  
  static int offset = 0;
  //looper gjennom hver pixel i stripen
  for(int i=0; i<strip.numPixels(); i++) {
    //regnbue effekten
    int hue = map((i + offset) % strip.numPixels(), 0, strip.numPixels(), 0, 65535); 
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue))); 
  }
  //Viser oppdaterte fargen til lysene
  strip.show();
  delay(timer);
  Serial.println("Mode: 3, Color: Rainbow");
  offset++; 
}

//Loop funksjon
void loop() {
  //Sjekker om programmet er startet
  if (!programStarted) {
    if (isButtonPressedFor(3000)) {
      //Hvis knappen er holdt nede i 3 sekunder, starter programmet.
      programStarted = true;
      Serial.println("Program Started");
      //Kjører startmodus funksjonen
      startmode();
    }
  } else {
    CheckmodeButton();
    
	//Les verdien fra lyssensoren, og juster brightness til Neopixel
    lightValue = analogRead(lightSensor);
    int brightness = map(lightValue, 0, 471, 0, 255);
    strip.setBrightness(brightness); 

    //Utfør riktig modus, basert på aktiv modus
    if (activeMode == 0) {
      mode1();
    } else if (activeMode == 1) {
      mode2();
    } else if (activeMode == 2) {
      mode3();
    }

    //Sjekk om fargeknappen er trykket, og endre farge om den er.
    if (digitalRead(colorButton) == LOW && !colorButtonPressed) {
      delay(50);
      ChangeColor();
      colorButtonPressed = true;
    } else if (digitalRead(colorButton) == HIGH) {
      colorButtonPressed = false;
    }

    Serial.println("Program Running");

    //Sjekk om knappen er holdt nede i 3 sek for å skru av.
    if (isButtonPressedFor(3000)) {
      programStarted = false;
      Serial.println("Program Stopped");
      lysAv();
    }
  }
}