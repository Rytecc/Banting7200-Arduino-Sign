#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>

#define OLED_ADDR 0x3C
#define PROGRAM_DEFAULTTICKDELAY 20

#define BTN_MODE 6
#define LIGHTS_LEDCOUNT 5  //every 3 lights is incremented as 1 LED (ex. value of 2 means 6 lights will light up)
#define LIGHTS_DATAPIN 7

long tickCount = 0;
int ledMode = 0;
int tickDelay = PROGRAM_DEFAULTTICKDELAY;

// This string array stores the effect names that will
// be displayed to the user. They also double as a means
// for indexing certain effects in the playEffects() function. (line 92)
String effectNames[5] = {
  "Red lights", //clear(0, 255, 0);
  "Blue lights", //clear(0, 0, 255);
  "Rainbow", //rainbowEffect();
  "Snake", //snakeEffect();
  "Knightrider" //knightRider();
};

Adafruit_SSD1306 display(-1);
CRGB leds[LIGHTS_LEDCOUNT];

// This ensures that the correct configuration is set in the SSD1306 library header file.
// The device being used has a height of 64 pixels. If it is not set in the header file, an error will be thrown.
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);

  //Initialise LEDs
  FastLED.addLeds<NEOPIXEL, LIGHTS_DATAPIN>(leds, LIGHTS_LEDCOUNT);

  //Initialise SSD1306 OLED display with a clear screen
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  //Initialise settings of the text, and where it will be appended on screen.
  display.setCursor(0, 30);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  int newMode = getCurrentMode(ledMode);
  playEffect(newMode);
  //Increment the ticking system and apply effects
  tickCount++;
  FastLED.show();

  // Delay for the specified tickDelay
  // Modifying the tickDelay integer will effectively modulate the speed of the program
  // Therefore making effects faster or slower.
  delay(tickDelay);
}

int getCurrentMode(int lastMode) {
  int mode = lastMode;
  if (isButtonPressed()) {
    // Increment ledMode
    mode++;

    // If the current mode is out of bounds of the max modes, reset to first mode.
    if (mode == sizeof(effectNames) / sizeof(*effectNames)) {
      mode = 0;
    }

    // Clear the display
    display.clearDisplay();
    display.display();

    // Display the current light effect being played.
    display.setCursor(0, 30);
    display.print("Current Mode:\n" + effectNames[ledMode]);
    display.display();
  }

  return mode;
}

void playEffects(int effectID) {
  switch(effectID) {
    // Play effects based on effectID
    // To see why it goes from 0 to 4, refer to line 20.
    case 0: clear(0, 255, 0); break;
    case 1: clear(0, 0, 255); break;
    case 2: rainbowEffect(); break;
    case 4: knightRider(); break;

    //For default cases, turn off light
    default: clear(0, 0, 0); break;
  }
}

void clear(int r, int g, int b) {
  //Set all leds to specified r, g, and b values.
  for (int i = 0; i < LIGHTS_LEDCOUNT; i++) {
    leds[i] = CRGB(r, g, b);
  }
}

int effect1_knightRiderPosition = 0;
int effect1_posDelta = 1;
void knightRider() {
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
void rainbowEffect() {
  //Get an initial hue using a sawtooth wave function 'beat8'
  int initHue = beat8(10, 255);

  //Fill all strips with a rainbow effect
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