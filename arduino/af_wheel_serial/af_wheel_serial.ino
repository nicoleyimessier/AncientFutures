#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// Array to hold RGB values
int rgb[6];
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {

  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  strip.begin();
  strip.setBrightness(50);
  strip.show();  // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 0, 0), 50);
}

void loop() {

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // Convert the inputString to a character array
    
    parseRGBValues(inputString, rgb);

    colorWipe(strip.Color(rgb[0], rgb[1], rgb[2]), 50);
    colorWipe(strip.Color(rgb[3], rgb[4], rgb[5]), 50);

    // clear the string:
    inputString = "";
    stringComplete = false;
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
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void parseRGBValues(const String& inputString, int rgb[6]) {
  // Convert the inputString to a character array
  char str[inputString.length() + 1];
  inputString.toCharArray(str, inputString.length() + 1);

  char *token = strtok(str, ",");  // Get the first token

  for (int index = 0; token != NULL && index < 6; ++index) {
    rgb[index] = atoi(token);  // Convert token to an integer and store it in the rgb array
    token = strtok(NULL, ","); // Get the next token
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
