#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);


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
    char str[inputString.length() + 1];
    inputString.toCharArray(str, inputString.length() + 1);

    // Array to hold RGB values
    int rgb[3];

    // Pointer to the tokens
    char *token;

    // Get the first token
    token = strtok(str, ",");

    // Index to keep track of the position in the rgb array
    int index = 0;

    // Loop through the tokens
    while (token != NULL) {
      // Convert token to an integer and store it in the rgb array
      rgb[index] = atoi(token);

      // Print each RGB value for debugging
      Serial.print("RGB[");
      Serial.print(index);
      Serial.print("] = ");
      Serial.println(rgb[index]);

      // Get the next token
      token = strtok(NULL, ",");

      // Increment the index
      index++;
    }

    colorWipe(strip.Color(rgb[0], rgb[1], rgb[2]), 50);

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

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
