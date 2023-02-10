#include <FastLED.h>
#define PROGRAM_TICKDELAY 20
#define NUM_LEDS 5  //every 3 lights is incremented as 1 LED (ex. value of 2 means 6 lights will light up)
#define DATA_PIN 8

#define BTN_EFFECT1 5
#define BTN_EFFECT2 6
#define BTN_EFFECT3 7

//Effect1 Settings (Effect1 is a knightrider light)
#define EFFECT1_TICKSDELAY 25

long tickCount = 0;
int ledMode = 0;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);

  // Set button pins to input
  for (int btn = 5; btn <= 7; btn++) {
    pinMode(btn, INPUT);
  }

  //Set LED signal pin to OUTPUT
  pinMode(8, OUTPUT);

  //Initialise LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  ledMode = getMode();
  switch (ledMode) {
    case 5:
      effect1();
      break;

    default:
      offMode();
      break;
  }

  tickCount++;
  Serial.println(digitalRead(7));
  delay(PROGRAM_TICKDELAY);
}

// Default lighting mode that turns off light
void offMode() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
}

int effect1_lastTickRun = 0;
int effect1_knightRiderPosition = 0;
int effect1_posDelta = 1;
void effect1() {
  //Wait a certain amount of ticks (20ms delays) before running
  if (tickCount - effect1_lastTickRun < EFFECT1_TICKSDELAY) {
    return;
  }

  //Set the current pointer LED to off
  leds[effect1_knightRiderPosition] = CRGB(0, 0, 0);

  //Based on position of LED pointer (leftbound or rightbound), make it go back or forward
  // forward if pointer was at beginning of light array
  // backward if pointer was at end of light array
  if (effect1_knightRiderPosition == NUM_LEDS - 1) { effect1_posDelta = -1; }
  if (effect1_knightRiderPosition == 0) { effect1_posDelta = 1; }
  effect1_knightRiderPosition += effect1_posDelta;

  //Set the light at now changed pointer to color
  leds[effect1_knightRiderPosition] = CRGB(255, 0, 0);

  //Push changes to LEDs
  FastLED.show();

  //Reset the tick counter
  effect1_lastTickRun = tickCount;
}

int getMode() {
  int mode = ledMode;
  for (int btn = 5; btn <= 7; btn++) {
    if (digitalRead(btn) == 1) {
      mode = btn;
    }
  }

  return mode;
}
