
/*
 * TODO: Implement pulsating LEDs
 * TODO: Implement signalling from Python back to Arduino when target time amount is reached
 */
 
// Input pins (Buttons):
const int btn1 = 2;
const int btn2 = 4;
const int btn3 = 8;
const int btn4 = 9;

// Output pins (LEDs):
const int led1 = 3;
const int led2 = 5;
const int led3 = 6;
const int led4 = 11;

int btns[] = {btn1, btn2, btn3, btn4};
int leds[] = {led1, led2, led3, led4};

// Variables:
int btn = 0;
int lastBTN = -1;
bool currentlyLogging = false;


void setup() {
 
  // define input and output pins, set initial LED state:
  for (int i = 0; i < 4; i++) {
    pinMode(btns[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Open Serial
  Serial.begin(9600);
  Serial.println("Serial logging now active.");
}

void loop() {
  /*
   * Serial Data: 
   * States: 1, 2, 3, 4 for logging in to the perspective states; 5, 6, 7, 8 for logging out.
   * Possible future feature: 9 and 0 sent from python to switch an LED signal if a certain target time has been reached.
   */
   
  btn = readBtns(); 
  if (btn != 0) {
    if (!currentlyLogging) {              //Being logged out means we're doing a simple log-in
      Serial.println(btn);
      lightsOn(btn);
      lastBTN = btn;
      currentlyLogging = true;
    } else {                              // Already logging means we're either logging out or switching to a different state.
      if (btn == lastBTN) {               // Same button means we're logging out.
        Serial.println(btn + 4);
        lightsOut();
        lastBTN = 0;
        currentlyLogging = false;
      } else {                            // Different button means switching states.
        Serial.println(lastBTN + 4);
        lightsOut();
        delay(100);
        Serial.println(btn);
        lightsOn(btn);
        lastBTN = btn;   
      }
    }
    delay(500);  // Debounce button. Don't need fancy millis() solutions because time is handled by the Python script and non-critical here.
  }
}


//*****************************************************
//                    Functions
//*****************************************************


int readBtns() {
// Reads all buttons and returns 0 for no button press, or 1 through 4 depending on which button was pressed.  
  for (int i = 0; i < 4; i++) {
    if(digitalRead(btns[i]) == LOW){
      return (i + 1);
    }
  }
  return 0;
}

void lightsOn(int btn) {
// Switches on a LED
  digitalWrite(leds[btn - 1], HIGH);  
}

void lightsOut() {
// Switches off all LEDs  
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
}  
