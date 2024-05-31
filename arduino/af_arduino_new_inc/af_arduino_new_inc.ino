#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 6
#define LEDS_PER_RING 16  // Number of LEDs in each ring
#define NUM_RINGS 18       // Total number of rings
#define BRIGHTNESS 255    // Set BRIGHTNESS to about 1/5 (max = 255)

// Calculate the total number of LEDs
#define TOTAL_LED_COUNT (LEDS_PER_RING * NUM_RINGS)

// Declare our NeoPixel strip object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// Serial
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// Volume serial
int volume = 0;

// Color storage
uint8_t start_r = 0;
uint8_t start_g = 0;
uint8_t start_b = 0;
bool onTransition = false;
int rgb[6] = { 0, 0, 0, 0, 0, 0 };

enum AnimationState {
  IDLE,
  RECORDING_COUNTDOWN,
  RECORDING,
  ANALYZING,
  TRANSITION_TO_SENTIMENT,
  ANIMATING,
  SINGLE_LED,
  TWO_LED,
  SINGLE_COLOR,
  SINGLE_COLOR_TO_GRADIENT,
  NO_ANIMATION,
  TRANSITION_TO_ATTRACT
};

AnimationState currentState = IDLE;

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);

  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  processSerialStateData();

  switch (currentState) {
    case RECORDING_COUNTDOWN:
      animate_gradient_fill(start_r, start_g, start_b, 10, 10, 10, 3000);
      currentState = RECORDING;
      break;
    case RECORDING:
      //strip.fill(strip.Color(volume, volume, volume));
      //strip.show();
      break;
    case ANALYZING:
      theaterChase(strip.Color(255, 255, 255), 500);
      break;
    case TRANSITION_TO_SENTIMENT:
      animate_gradient_fill(start_r, start_g, start_b, 255, 255, 255, 1000);
      currentState = IDLE;
      break;
    case ANIMATING:
      if (onTransition) {
        convertColorToRGB(strip.getPixelColor(0));
        animate_gradient_fill(start_r, start_g, start_b, rgb[0], rgb[1], rgb[2], 1000);
        onTransition = false;
      }

      pulseBetweenColors(rgb[0], rgb[1], rgb[2], rgb[3], rgb[4], rgb[5], 2000);
      break;
    case TRANSITION_TO_ATTRACT:
      animate_gradient_fill(start_r, start_g, start_b, 255, 95, 50, 1000);
      currentState = IDLE;
      break;
    case IDLE:
      pulseBetweenColors(255, 95, 50, 255, 255, 255, 3000);
      break;
    case SINGLE_LED:
      turnOnNLedOfEachRing(1);
      break;
    case TWO_LED:
      turnOnNLedOfEachRing(2);
      break;
    case SINGLE_COLOR:
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      colorWipe(strip.Color(rgb[0], rgb[1], rgb[2]), 50);
      currentState = NO_ANIMATION;
      break;
    case SINGLE_COLOR_TO_GRADIENT:
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      colorWipeBothRows(strip.Color(rgb[0], rgb[1], rgb[2]), 30);
      currentState = ANIMATING;
      break;
    case NO_ANIMATION:
      break;
    default:
      pulseBetweenColors(255, 95, 50, 255, 255, 255, 3000);
      break;
  }
}

// SerialEvent occurs whenever new data comes in the hardware serial RX.
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void processSerialStateData() {
  if (stringComplete) {
    Serial.print(inputString);

    if (inputString.indexOf('z') >= 0) {
      currentState = SINGLE_LED;
    } else if (inputString.indexOf('y') >= 0) {
      currentState = TWO_LED;
    } else if (inputString.indexOf('a') >= 0) {
      currentState = ANALYZING;
    } else if (inputString.indexOf('r') >= 0) {
      currentState = RECORDING_COUNTDOWN;
    } else if (inputString.indexOf('s') >= 0) {
      currentState = TRANSITION_TO_ATTRACT;
    } else if (inputString.indexOf('v') >= 0) {
      String mapping = inputString.substring(1);
      volume = mapping.toInt();
      currentState = RECORDING;
    } else if (inputString.indexOf('c') >= 0) {
      parseRGBValuesSingleColor(inputString, rgb);
      currentState = SINGLE_COLOR;
    } else if (inputString.indexOf('g') >= 0) {
      parseRGBValuesSingleColor(inputString, rgb);
      currentState = SINGLE_COLOR_TO_GRADIENT;
    } else {
      parseRGBValues(inputString, rgb);
      onTransition = true;
      currentState = ANIMATING;
    }

    inputString = "";
    stringComplete = false;
  }
}

void parseRGBValuesSingleColor(const String& inputString, int rgb[6]) {
  char str[inputString.length() + 1];
  inputString.toCharArray(str, inputString.length() + 1);

  char* token = strtok(str, ",");
  for (int index = 0; token != NULL && index < 6; ++index) {
    rgb[index] = atoi(token);
    token = strtok(NULL, ",");
  }

  // Ensure all values are in the 0-255 range
  for (int i = 0; i < 6; i++) {
    if (rgb[i] < 0) rgb[i] = 0;
    if (rgb[i] > 255) rgb[i] = 255;
  }
}

void parseRGBValues(const String& inputString, int rgb[6]) {
  char str[inputString.length() + 1];
  inputString.toCharArray(str, inputString.length() + 1);

  char* token = strtok(str, ",");
  for (int index = 0; token != NULL && index < 6; ++index) {
    rgb[index] = atoi(token);
    token = strtok(NULL, ",");
  }

  // Ensure all values are in the 0-255 range
  for (int i = 0; i < 6; i++) {
    if (rgb[i] < 0) rgb[i] = 0;
    if (rgb[i] > 255) rgb[i] = 255;
  }
}

void pulseBetweenColors(uint8_t start_r, uint8_t start_g, uint8_t start_b,
                        uint8_t end_r, uint8_t end_g, uint8_t end_b,
                        int duration) {
  animate_gradient_fill(start_r, start_g, start_b, end_r, end_g, end_b, duration);
  animate_gradient_fill(end_r, end_g, end_b, start_r, start_g, start_b, duration);
}

void storeColorMix(uint8_t r, uint8_t g, uint8_t b) {
  start_r = r;
  start_g = g;
  start_b = b;
}

void convertColorToRGB(uint32_t color) {
  start_r = (color >> 16) & 0xFF;
  start_g = (color >> 8) & 0xFF;
  start_b = color & 0xFF;
}

void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) {
    for (int b = 0; b < 3; b++) {
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);
      }
      strip.show();


      serialEvent();

      if (stringComplete) return;
      delay(wait);
    }
  }
}

void animate_gradient_fill(uint8_t start_r, uint8_t start_g, uint8_t start_b,
                           uint8_t end_r, uint8_t end_g, uint8_t end_b,
                           int duration_ms) {
  unsigned long start = millis();
  strip.fill(strip.Color(start_r, start_g, start_b));
  strip.show();
  unsigned long delta = millis() - start;
  while (delta < duration_ms) {
    serialEvent();
    if (stringComplete) return;
    float pos = (float)delta / (float)duration_ms;
    uint32_t color = color_gradient(start_r, start_g, start_b, end_r, end_g, end_b, pos);
    strip.fill(color);
    strip.show();
    delta = millis() - start;
  }
  strip.fill(strip.Color(end_r, end_g, end_b));
  strip.show();
}

uint32_t color_gradient(uint8_t start_r, uint8_t start_g, uint8_t start_b,
                        uint8_t end_r, uint8_t end_g, uint8_t end_b,
                        float pos) {
  uint8_t red = (uint8_t)lerp(pos, 0.0, 1.0, start_r, end_r);
  uint8_t green = (uint8_t)lerp(pos, 0.0, 1.0, start_g, end_g);
  uint8_t blue = (uint8_t)lerp(pos, 0.0, 1.0, start_b, end_b);
  storeColorMix(red, green, blue);
  return Adafruit_NeoPixel::Color(red, green, blue);
}

float lerp(float x, float x0, float x1, float y0, float y1) {
  x = x > x1 ? x1 : x;
  x = x < x0 ? x0 : x;
  return y0 + (y1 - y0) * ((x - x0) / (x1 - x0));
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipeBothRows(uint32_t c, uint8_t wait) {

  for (uint16_t i = 0; i < strip.numPixels()/2; i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(strip.numPixels()-i, c);
    strip.show();
    delay(wait);
  }
}


void turnOnNLedOfEachRing(int numLEDs) {
  // Define the color (e.g., red)
  uint32_t color = strip.Color(255, 0, 0);

  strip.fill(strip.Color(0, 0, 0));
  strip.show();

  // Loop through each ring and turn on the first LED
  for (int i = 0; i < NUM_RINGS; i++) {
    for (int j = 0; j < numLEDs; j++)
      strip.setPixelColor(i * LEDS_PER_RING + j, color);
  }

  // Show the changes on the strip
  strip.show();
}
