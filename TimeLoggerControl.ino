/*
 * TODO: Implement Task management
 * TODO: Implement signalling from Python back to Arduino (when target time is met)
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
const int indicator = 10;

int btns[] = {btn1, btn2, btn3, btn4};
int leds[] = {led1, led2, led3, led4, indicator};

// NOTE: ((stepper^2) * .1), rounded down
int fadeLUT[75] =   {0,   0,   0,   1,   2,   3,   4,   6,   8,  10,
                    12,  14,  16,  19,  22,  25,  28,  32,  36,  40,
                    44,  48,  52,  57,  62,  67,  72,  78,  84,  90,
                    96, 102, 108, 115, 122, 129, 136, 144, 136, 129,
                   122, 115, 108, 102,  96,  90,  84,  78,  72,  67,
                    62,  57,  52,  48,  44,  40,  36,  32,  28,  25,
                    22,  19,  16,  14,  12,  10,   8,   6,   4,   3,
                     2,   1,   0,   0,   0};
int flashLUT[75] = {80,  80,  80,   0,   0,  80,  80,  80,   0,   0,
                     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                     0,   0,   0,   0,   0};

// Variables:
bool currentlyLogging = false;
int btn = 0;
int lastBTN = -1;
int btnsLength = sizeof(btns)/sizeof(btns[0]);
int ledsLength = sizeof(leds)/sizeof(leds[0]);
int fadeLUTlength = sizeof(fadeLUT)/sizeof(fadeLUT[0]);
int flashLUTlength = sizeof(flashLUT)/sizeof(flashLUT[0]);

// Variables for task management logic:
int interval = 40;
/* Possible interval values:  
 *  20msec: 50 times/second
 *  25msec: 40 times/second
 *  40msec: 25 times/second
 */ 
 
unsigned long currentTime = 0;
unsigned long oldTime = -interval;
int stepper = 0;
int maxStepValue = 75; // (enough steps to cover the 3 or so seconds of the fade interval)

 

void setup() {
 
  // define input and output pins, set initial LED state:
  for (int i = 0; i < btnsLength; i++) {
    pinMode(btns[i], INPUT_PULLUP);
  }
  for (int i = 0; i < ledsLength; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  
  // Open Serial
  Serial.begin(9600);
}

void loop() {
  /*
   * Serial Data: 
   * States: 1, 2, 3, 4 for logging in to the perspective states; 5, 6, 7, 8 for logging out.
   * Possible future feature: 9 and 0 sent from python to switch an LED signal if a certain target time has been reached.
   
 
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
        delay(200);                       // Time for python script to read
        Serial.println(btn);
        lightsOn(btn);
        lastBTN = btn;   
      }
    }
    delay(500);  // Debounce button. Don't need fancy millis() solutions because time is handled by the Python script and non-critical here.
  } */
  
  currentTime = millis();
  if (currentTime >= (oldTime + interval)) {
    oldTime = currentTime;
    
    callAllFunctions(stepper);
    
    if (stepper >= maxStepValue-1) {
      stepper = 0;
    } else {
      stepper++;
    }
  }
}


//*****************************************************
//                    Functions
//*****************************************************

void callAllFunctions(int stepper) {
  analogWrite(leds[4], flashLUT[stepper]); 
  analogWrite(leds[2], fadeLUT[stepper]);
  
}

int readBtns() {
// Checks all buttons and returns 0 for no button press, or 1 through 4 depending on which button was pressed.  
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
