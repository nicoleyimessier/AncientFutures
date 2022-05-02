
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6
//#define LED_COUNT  26
#define LED_COUNT  16
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Serial
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool animatePositive = false;
bool animateNegative = false;
bool animateNetrual = false;
bool recording = false;
bool analyzing = false;
bool animate = false;

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.print(inputString);

    // RESET ALL VALUES
    animatePositive = false;
    animateNegative = false;
    animateNetrual = false;
    recording = false;
    analyzing = false;

    if (inputString.indexOf('p') >= 0) {
      animatePositive = true;
      animate = true;
    } else if (inputString.indexOf('n') >= 0) {
      animateNegative = true;
      animate = true;
    } else if (inputString.indexOf('c') >= 0) {
      animateNetrual = true;
      animate = true;
    } else if (inputString.indexOf('a') >= 0) {
      analyzing = true;
      animate = true;
    } else if (inputString.indexOf('r') >= 0) {
      recording = true;
      animate = true;
    } else if (inputString.indexOf('s') >= 0) {
      animatePositive = false;
      animate = false;
    }
    // clear the string:
    inputString = "";
    stringComplete = false;


    // animate = !animate;
  }

  if (animate)
  {
    if (recording) {
      strip.fill(strip.Color( 0, 0, 0));
      strip.show();
    } else if (analyzing)
      theaterChase(strip.Color(255, 255, 255), 50);
    else if (animatePositive)
      pulseBlue(5);
    else if (animateNegative)
      pulseRed(5);
    else if (animateNetrual)
      pulseNeutral(5);


  }
  else
  {
    colorMix(5); 
  }
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
    } else
    {
      inputString += inChar;
    }
  }
}

void pulseRed(uint8_t wait) {
  for (int j = 0; j < 256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( j, 0, 0));
    strip.show();
    delay(wait);
  }

  for (int j = 255; j >= 0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( j, 0, 0));
    strip.show();
    delay(wait);
  }
}


void pulseBlue(uint8_t wait) {
  for (int j = 0; j < 256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( 0, 0, j));
    strip.show();
    delay(wait);
  }

  for (int j = 255; j >= 0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( 0, 0, j));
    strip.show();
    delay(wait);
  }
}

void pulseNeutral(uint8_t wait) {
  //255, 95, 50
  for (int j = 0; j < 256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( j, map(j, 0, 255, 0, 95), map(j, 0, 255, 0, 50)));
    strip.show();
    delay(wait);
  }

  for (int j = 255; j >= 0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( j, map(j, 0, 255, 0, 95), map(j, 0, 255, 0, 50)));
    strip.show();
    delay(wait);
  }
}


void colorMix(uint8_t wait) {
  for (int j = 0; j < 256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( 255, map(j, 0, 255,  95, 255), map(j, 0, 255, 50, 255)));
    strip.show();

    if(stringComplete) return; 
    delay(wait);
  }

  for (int j = 255; j >= 0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( 255, map(j, 0, 255, 95, 255), map(j, 0, 255, 50, 255)));
    strip.show();

    if(stringComplete) return; 
    delay(wait);
  }

  delay(1000);
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}
