#include <stdio.h>

// Input pins (Buttons):
const int btn1 = 2;
const int btn2 = 4;
const int btn3 = 8;
const int btn4 = 9;
int btns[] = {btn1, btn2, btn3, btn4};

// Output pins (LEDs):
const int led1 = 3;
const int led2 = 5;
const int led3 = 6;
const int led4 = 11;
int leds[] = {led1, led2, led3, led4};

// Variables:
int btnState = 0;


void setup() {
 
  // define input and output pins, set initial LED state:
  for (int i = 0; i < 4; i++) {
    pinMode(btns[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Open Serial
  Serial.begin(9600);
  Serial.println("Serial is now active.");
}

void loop() {
  
  btnState = readBtns();
  if (btnState != 0) {
    Serial.print("Button ");
    Serial.print(btnState);
    Serial.print(" was pressed.\n");
    digitalWrite(leds[btnState - 1], HIGH);
    delay(800); 
    lightsOut();
  }
}


//*****************************************************
//                    Functions
//*****************************************************


int readBtns() {
// Reads all buttons and returns 1, 2, 3, or 4 depending on which button was pressed, 0 if no button press is detected.
  for (int i = 0; i < 4; i++) {
    if(digitalRead(btns[i]) == LOW){        //DEBUG: Serial.print("A button was pressed. ");
      return (i + 1);
    }                                       //DEBUG: else {Serial.print("No button press detected.\n");}
  }
  return 0;
}


void lightsOut() {
// Switches off all LEDs  
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
}  
