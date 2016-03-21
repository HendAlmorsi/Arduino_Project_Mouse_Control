// include HID library
#include <HID.h>


// set pin numbers
const int mouseButton = 3;    // input pin for the mouse pushButton
const int rightButton = 4;

const int xAxis = A0;         // joystick X axis
const int yAxis = A1;         // joystick Y axis

// parameters for reading the joystick:
int range = 5;                // output range of X or Y movement
int responseDelay = 2;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold(the velocity required for acceleration to become effective)
int center = range / 2;       // resting position value

void setup() {

  pinMode(rightButton, INPUT);  // Right click pin
  pinMode(mouseButton, INPUT);  // Left click pin

  // Starts Serial at baud 115200 otherwise HID wont work on Uno/Mega.
  // This is not needed for Leonado/(Pro)Micro but make sure to activate desired USB functions in HID.h
  Serial.begin(SERIAL_HID_BAUD);
  
  // take control of the mouse:
  Mouse.begin();
}

void loop() {
 
  // read and scale the two axes:
  int xReading = readAxis(0);
  int yReading = readAxis(1);

  // move command
  Mouse.move(xReading, -yReading, 0); // as y axis gets inverted

  // read the mouse button and click or not click:
  // if the mouse button is pressed:
  if (digitalRead(mouseButton) == LOW) {
    // if the mouse is not pressed, press it:
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    }
  }
  // else the mouse button is not pressed:
  else {
    // if the mouse is pressed, release it:
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT);
    }
  }
   // if the right mouse button is pressed:
  if (digitalRead(rightButton) == LOW) {
    // if the mouse is not pressed, press it:
    if (!Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.press(MOUSE_RIGHT);
    }
  }
  // else the mouse button is not pressed:
  else {
    // if the mouse is pressed, release it:
    if (Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.release(MOUSE_RIGHT);
    }
  }

  delay(responseDelay);
}

/*
  reads an axis (0 or 1 for x or y) and scales the
 analog input range to a range from 0 to <range>
 */
int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the
  // rest position threshold,  use it
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis
  return distance;
}
