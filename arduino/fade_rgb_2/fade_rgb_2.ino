#define GREEN 10
#define BLUE 9
#define RED 11
#define delayTime 20

void setup() {

  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, HIGH);
}

int redVal;
int blueVal;
int greenVal;

void loop() {


  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(GREEN, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

// fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(GREEN, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
  
}
