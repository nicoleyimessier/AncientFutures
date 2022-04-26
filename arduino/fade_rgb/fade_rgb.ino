
// variables to hold the LED color
int rVal = 254;
int gVal = 1;
int bVal = 127;

int rDir = -1;
int gDir = 1;
int bDir = -1;

// constants to name the pins
const int rPin = 6;
const int gPin = 10;
const int bPin = 9;

void setup() {
  // declare the pinModes
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
}

void loop() {
  // PWM the LED
  // when using a common anode RGB LED like the ones in
  // your kits, you create a voltage difference across
  // each diode to light up the LED, that is, a PWM value
  // of 255 will turn that light off, while a PWM value of 0
  // will turn that light on fully.

  analogWrite(rPin, rVal);
  analogWrite(gPin, gVal);
  analogWrite(bPin, bVal);

  // change the values of the LEDs
  rVal = rVal + rDir;
  gVal = gVal + gDir;
  bVal = bVal + bDir;

  // for each color, change direction if
  // you reached 0 or 255
  if (rVal >= 255 || rVal <= 0) {
    rDir = rDir * -1;
  }

  if (gVal >= 255 || gVal <= 0) {
    gDir = gDir * -1;
  }

  if (bVal >= 255 || bVal <= 0) {
    bDir = bDir * -1;
  }

  // slight delay so it doesn't rotate color too quicky
  delay(33);
}
