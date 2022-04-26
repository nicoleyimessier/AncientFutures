
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6
//#define LED_COUNT  26
#define LED_COUNT  1
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

// Serial
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool animatePositive = false; 
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

    if(inputString.indexOf('p') >= 0) {
      animatePositive = true; 
       animate = true; 
    } else if(inputString.indexOf('n') >= 0) {
      animatePositive = false; 
       animate = true; 
    }else if(inputString.indexOf('s') >= 0) {
      animatePositive = false; 
       animate = false; 
    }
    // clear the string:
    inputString = "";
    stringComplete = false;

   
   // animate = !animate;                
  }

  if(animate)
  {
    if(animatePositive) 
    {
      pulseWhite(5);
    }
    else 
    {
      pulseRed(5);
    }
  } 
  else 
  {
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(0)));
    strip.show(); 
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
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(255, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(255, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}
