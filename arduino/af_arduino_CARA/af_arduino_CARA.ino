#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 6
#define BRIGHTNESS 255  // Set BRIGHTNESS to about 1/5 (max = 255)

// Calculate the total number of LEDs
#define TOTAL_LED_COUNT 60

// Declare our NeoPixel strip object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LED_COUNT, PIN, NEO_RGB + NEO_KHZ800);

// Color storage
uint8_t start_r = 0;
uint8_t start_g = 0;
uint8_t start_b = 0;
bool onTransition = false;
int rgb[6] = { 0, 0, 0, 0, 0, 0 };

// Define a set of 100 colors
const uint32_t colorSet[100] = {
  0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF, 0xC0C0C0, 0x808080, 0x800000,
  0x808000, 0x008000, 0x800080, 0x008080, 0x000080, 0xFFA500, 0xA52A2A, 0xDEB887, 0x5F9EA0, 0x7FFF00,
  0xD2691E, 0xFF7F50, 0x6495ED, 0xFFF8DC, 0xDC143C, 0x00FFFF, 0x00008B, 0x008B8B, 0xB8860B, 0xA9A9A9,
  0x006400, 0xBDB76B, 0x8B008B, 0x556B2F, 0xFF8C00, 0x9932CC, 0x8B0000, 0xE9967A, 0x8FBC8F, 0x483D8B,
  0x2F4F4F, 0x00CED1, 0x9400D3, 0xFF1493, 0x00BFFF, 0x696969, 0x1E90FF, 0xB22222, 0xFFFAF0, 0x228B22,
  0xFF00FF, 0xDCDCDC, 0xF8F8FF, 0xFFD700, 0xDAA520, 0x808080, 0x008000, 0xADFF2F, 0xF0FFF0, 0xFF69B4,
  0xCD5C5C, 0x4B0082, 0xFFFFF0, 0xF0E68C, 0xE6E6FA, 0xFFF0F5, 0x7CFC00, 0xFFFACD, 0xADD8E6, 0xF08080,
  0xE0FFFF, 0xFAFAD2, 0xD3D3D3, 0x90EE90, 0xFFB6C1, 0xFFA07A, 0x20B2AA, 0x87CEFA, 0x778899, 0xB0C4DE,
  0xFFFFE0, 0x00FF00, 0x32CD32, 0xFAF0E6, 0xFF00FF, 0x800000, 0x66CDAA, 0x0000CD, 0xBA55D3, 0x9370DB,
  0x3CB371, 0x7B68EE, 0x00FA9A, 0x48D1CC, 0xC71585, 0x191970, 0xF5FFFA, 0xFFE4E1, 0xFFE4B5, 0xFFDEAD
};

enum AnimationState {
  IDLE,
  SINGLE_COLOR_TO_GRADIENT,
  ANIMATING,
  TRANSITION_TO_ATTRACT
};

AnimationState currentState = IDLE;
unsigned long stateStartTime = 0;

void setup() {
  Serial.begin(9600);

  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  pinMode(LED_BUILTIN, OUTPUT);

  randomSeed(analogRead(0));  // Seed the random number generator
  updateRandomColors();
  stateStartTime = millis();
}

void loop() {
  // 60 seconds 60000
  if (millis() - stateStartTime >= 60000) {  // Check if 60 seconds have passed
    updateRandomColors();                    // Update to new random colors
    moveToNextState();
    stateStartTime = millis();
  }

  switch (currentState) {
    case IDLE:
      Serial.println("IDLE");
      pulseBetweenColors(255, 95, 50, 255, 255, 255, 3000);
      break;
    case SINGLE_COLOR_TO_GRADIENT:
      Serial.println("SINGLE_COLOR_TO_GRADIENT");
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      colorWipe(strip.Color(rgb[0], rgb[1], rgb[2]), 30);
      currentState = ANIMATING; 
      break;
    case ANIMATING:
      Serial.println("ANIMATING");
      if (onTransition) {
        convertColorToRGB(strip.getPixelColor(0));
        animate_gradient_fill(start_r, start_g, start_b, rgb[0], rgb[1], rgb[2], 1000);
        onTransition = false;
      }

      pulseBetweenColors(rgb[0], rgb[1], rgb[2], rgb[3], rgb[4], rgb[5], 2000);
      break;
    case TRANSITION_TO_ATTRACT:
      Serial.println("TRANSITION_TO_ATTRACT");
      animate_gradient_fill(start_r, start_g, start_b, 255, 95, 50, 1000);
      currentState = IDLE; 
      break;
    default:
      pulseBetweenColors(255, 95, 50, 255, 255, 255, 3000);
      break;
  }
}

void moveToNextState() {
  switch (currentState) {
    case IDLE:
      currentState = SINGLE_COLOR_TO_GRADIENT;
      break;
    case ANIMATING:
      currentState = TRANSITION_TO_ATTRACT;
      break;
    default:
      break;
  }
}


void updateRandomColors() {
  // Select two random colors from the color set
  uint32_t color1 = colorSet[random(100)];
  uint32_t color2 = colorSet[random(100)];

  // Split the first color into its RGB components
  rgb[0] = (color1 >> 16) & 0xFF;
  rgb[1] = (color1 >> 8) & 0xFF;
  rgb[2] = color1 & 0xFF;

  // Split the second color into its RGB components
  rgb[3] = (color2 >> 16) & 0xFF;
  rgb[4] = (color2 >> 8) & 0xFF;
  rgb[5] = color2 & 0xFF;
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

void animate_gradient_fill(uint8_t start_r, uint8_t start_g, uint8_t start_b,
                           uint8_t end_r, uint8_t end_g, uint8_t end_b,
                           int duration_ms) {
  unsigned long start = millis();
  strip.fill(strip.Color(start_r, start_g, start_b));
  strip.show();
  unsigned long delta = millis() - start;
  while (delta < duration_ms) {
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
