
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#include <avr/wdt.h>
#endif

#define PIN 6
//#define LED_COUNT  26
#define LED_COUNT 304
#define BRIGHTNESS 255  // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);


// Serial
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool animatePositive = false;
bool animateNegative = false;
bool animateNetrual = false;
bool recordingCountdown = false;
bool recording = false;
bool analyzing = false;
bool animate = false;
bool transitionToSentimentAnimation = false;
bool transitionToAttract = false;

//volume serial
int volume = 0;

//Color storage
uint8_t start_r = 0;
uint8_t start_g = 0;
uint8_t start_b = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  wdt_enable(WDTO_8S);
}

void loop() {
  processSerialStateData();

  if (animate) {
    if (recordingCountdown) {
      animate_gradient_fill(start_r, start_g, start_b, 0, 0, 0, 3000);
      recording = true;
      recordingCountdown = false;
    } else if (recording) {
      //strip.fill(strip.Color(0, 0, 0));
      //strip.show();
      strip.fill(strip.Color(volume, volume, volume));
      strip.show();
    } else if (analyzing)
      theaterChase(strip.Color(255, 255, 255), 500);
    else if (transitionToSentimentAnimation) {
      animate_gradient_fill(start_r, start_g, start_b, 255, 255, 255, 1000);
      transitionToSentimentAnimation = false;
    } else if (animatePositive)
      pulseBlue(2000);
    else if (animateNegative)
      pulseRed(2000);
    else if (animateNetrual)
      pulseNeutral(2000);
    else if (transitionToAttract) {
      animate_gradient_fill(start_r, start_g, start_b, 255, 95, 50, 1000);
      animate = false;
      transitionToAttract = false;
    }
  } else {
    animate_gradient_fill(255, 95, 50, 255, 255, 255, 3000);
    animate_gradient_fill(255, 255, 255, 255, 95, 50, 3000);
  }
  wdt_reset();
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:

    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}



void processSerialStateData() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.print(inputString);

    // RESET ALL VALUES
    animatePositive = false;
    animateNegative = false;
    animateNetrual = false;
    recordingCountdown = false;
    recording = false;
    analyzing = false;
    transitionToSentimentAnimation = false;
    transitionToAttract = false;

    if (inputString.indexOf('p') >= 0) {
      transitionToSentimentAnimation = true;
      animatePositive = true;
      animate = true;
    } else if (inputString.indexOf('n') >= 0) {
      transitionToSentimentAnimation = true;
      animateNegative = true;
      animate = true;
    } else if (inputString.indexOf('c') >= 0) {
      transitionToSentimentAnimation = true;
      animateNetrual = true;
      animate = true;
    } else if (inputString.indexOf('a') >= 0) {
      analyzing = true;
      animate = true;
    } else if (inputString.indexOf('r') >= 0) {
      recordingCountdown = true;
      animate = true;
    } else if (inputString.indexOf('s') >= 0) {
      transitionToAttract = true;
      animate = true;
      //animate = false;
    } else if (inputString.indexOf('v') >= 0) {
      //volume = 255;
      animate = true; 
      recording = true; 

      String mapping = ""; 
      for(int i=1; i < inputString.length(); i++)
      {
        mapping += inputString[i]; 
      }

      volume = mapping.toInt(); 
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    
  }
}

void pulseRed(int duration) {
  animate_gradient_fill(255, 255, 255, 255, 0, 0, duration);
  animate_gradient_fill(255, 0, 0, 255, 255, 255, duration);
}


void pulseBlue(int duration) {
  animate_gradient_fill(255, 255, 255, 0, 0, 255, duration);
  animate_gradient_fill(0, 0, 255, 255, 255, 255, duration);
}

void pulseNeutral(int duration) {
  animate_gradient_fill(255, 255, 255, 255, 95, 50, duration);
  animate_gradient_fill(255, 95, 50, 255, 255, 255, duration);
}

void storeColorMix(uint8_t r,
                   uint8_t g,
                   uint8_t b) {
  start_r = r;
  start_g = g;
  start_b = b;
}


// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) {   // Repeat 10 times...
    for (int b = 0; b < 3; b++) {  //  'b' counts from 0 to 2...
      strip.clear();               //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);  // Set pixel 'c' to value 'color'
      }
      strip.show();  // Update strip with new contents

      serialEvent();
      if (stringComplete) {
        return;
      }

      delay(wait);  // Pause for a moment
    }
  }
}


// ANIMATIONS//Linaer interpolate
void animate_gradient_fill(
  uint8_t start_r,
  uint8_t start_g,
  uint8_t start_b,
  uint8_t end_r,
  uint8_t end_g,
  uint8_t end_b,
  int duration_ms) {

  unsigned long start = millis();
  // start color
  strip.fill(strip.Color(start_r, start_g, start_b));
  strip.show();

  // start time
  unsigned long delta = millis() - start;

  // animation loop
  while (delta < duration_ms) {
    wdt_reset();
    serialEvent();
    if (stringComplete) {
      return;
    }
    float pos = (float)delta / (float)duration_ms;
    uint32_t color = color_gradient(start_r, start_g, start_b, end_r, end_g, end_b, pos);
    strip.fill(color);
    strip.show();
    delta = millis() - start;
  }

  //end color
  strip.fill(strip.Color(end_r, end_g, end_b));
  strip.show();
}

uint32_t color_gradient(uint8_t start_r,
                        uint8_t start_g,
                        uint8_t start_b,
                        uint8_t end_r,
                        uint8_t end_g,
                        uint8_t end_b,
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
