#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>

#define OLED_ADDR 0x3C
#define PROGRAM_TICKDELAY 20

#define BTN_MODE 6
#define LIGHTS_LEDCOUNT 5  //every 3 lights is incremented as 1 LED (ex. value of 2 means 6 lights will light up)
#define LIGHTS_DATAPIN 7

//Effect1 Settings (Effect1 is a knightrider light)
#define EFFECT1_TICKSDELAY 25

long tickCount = 0;
int ledMode = 0;
String effectNames[5] = {
  "Red lights",
  "Blue lights",
  "Rainbow",
  "Snake",
  "Knightrider"
};

Adafruit_SSD1306 display(-1);
CRGB leds[LIGHTS_LEDCOUNT];

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);

  //Initialise LEDs
  FastLED.addLeds<NEOPIXEL, LIGHTS_DATAPIN>(leds, LIGHTS_LEDCOUNT);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  display.setCursor(0, 30);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  if (isButtonPressed()) {
    ledMode++;

    if (ledMode == 6) {
      ledMode = 0;
    }

    display.clearDisplay();
    display.display();

    display.setCursor(0, 30);
    display.print("Current Mode:\n" + effectNames[ledMode]);
    display.display();
  }

  Serial.println(ledMode);
  tickCount++;
  clear(0, 0, 0);
  FastLED.show();
  delay(PROGRAM_TICKDELAY);
}

// Default lighting mode that turns off light
void clear(int r, int g, int b) {
  for (int i = 0; i < LIGHTS_LEDCOUNT; i++) {
    leds[i] = CRGB(r, g, b);
  }
}

int effect1_knightRiderPosition = 0;
int effect1_posDelta = 1;
void effect1() {
  //Wait a certain amount of ticks (20ms delays) before running
  if (tickCount % 20 != 0) {
    return;
  }

  //Set the current pointer LED to off
  clear(255, 0, 0);

  //Based on position of LED pointer (leftbound or rightbound), make it go back or forward
  // forward if pointer was at beginning of light array
  // backward if pointer was at end of light array
  if (effect1_knightRiderPosition == LIGHTS_LEDCOUNT - 1) { effect1_posDelta = -1; }
  if (effect1_knightRiderPosition == 0) { effect1_posDelta = 1; }
  effect1_knightRiderPosition += effect1_posDelta;

  //Set the light at now changed pointer to color
  leds[effect1_knightRiderPosition] = CRGB(255, 0, 255);
}

int effect2_rainbowFirstPixelHue = 0;
void effect2() {
  int initHue = beat8(10, 255);
  fill_rainbow(leds, LIGHTS_LEDCOUNT, initHue, 10);
}

bool buttonReady = false;
bool isButtonPressed() {
  bool pressed = digitalRead(BTN_MODE);
  if (pressed == HIGH) {
    if (buttonReady) {
      buttonReady = false;
      return true;
    }
  } else {
    buttonReady = true;
  }

  return false;
}