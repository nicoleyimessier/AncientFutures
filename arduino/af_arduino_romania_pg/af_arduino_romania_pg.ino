// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 6 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  strip.clear(); // Set all pixel colors to 'off'

//  // The first NeoPixel in a strand is #0, second is 1, all the way up
//  // to the count of pixels minus one.
//  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
//
//    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
//    // Here we're using a moderately bright green color:
//    strip.setPixelColor(i, strip.Color(0, 150, 0));
//
//    strip.show();   // Send the updated pixel colors to the hardware.
//
//    delay(DELAYVAL); // Pause before next pass through loop
//  }

  // strip.clear(); 
  //colorWipe(strip.Color(127, 255, 255), 50);
  //strip.setPixelColor(0, strip.Color(127, 127, 127)); 
  //strip.show();
 

  //strip.clear(); 
  
  //negative
  //pulseRed(3); 
  //delay(2000); 

//positive
//  pulseBlue(3); 

// processing
  //theaterChase(strip.Color(255, 255, 255), 50); // White, half brightness

 // colorWipe(strip.Color(50,   50,   50), 1000); // white

  // strip.fill(strip.Color( 255, 95, 50)); // netural color
  
    //strip.fill(strip.Color( 255, 255, 255));
  // strip.show();
  //pulseNeutral(3); 
//delay(1000); 
colorMix(10); 
//delay(1000); 
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void colorMix(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( 255, map(j, 0, 255,  95, 255), map(j, 0, 255, 50, 255)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( 255, map(j, 0, 255, 95, 255), map(j, 0, 255, 50, 255)));
    strip.show();
    delay(wait);
  }

  delay(1000); 
}

void pulseRed(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( j, 0, 0));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( j, 0, 0));
    strip.show();
    delay(wait);
  }
}


void pulseNeutral(uint8_t wait) {

  //255, 95, 50
  
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( j, map(j, 0, 255, 0, 95), map(j, 0, 255, 0, 50)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( j, map(j, 0, 255, 0, 95), map(j, 0, 255, 0, 50)));
    strip.show();
    delay(wait);
  }
}

void pulseBlue(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    strip.fill(strip.Color( 0, 0, j));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color( 0, 0, j));
    strip.show();
    delay(wait);
  }
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
