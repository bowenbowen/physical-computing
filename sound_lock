/******************************************************************************
   INTRO:
   This is a lock system that unlocks at the play of the chosen song.
   The sound sonsor is used to monitor the volume of the sounds. 
   If the reading contains a pattern from the chosen song, the system get unlocked.
   It is not very accurate though. You might have to replay the chosen part several times to unlock.
   You also have to play the song in a quite environment to get rid of the influences from outside noises.
 ******************************************************************************/

/******************************************************************************
   ISSUES:
   when LEDs are added to the circuit the readings of the sonsor tend to overall increase by a certain amount.
   Current solution is to use an offset variable to calibrate the overall reading.
 ******************************************************************************/
 
/******************************************************************************
   Based on sound_detector_demo.ino from https://learn.sparkfun.com/tutorials/sound-detector-hookup-guide/all

   Connections:
   The Sound Detector is connected to the Adrduino as follows:
   (Sound Detector -> Arduino pin)
   GND → GND
   VCC → 5V
   Gate → Pin 2
   Envelope → A0

   Resources:
   Additional library requirements: none

   Development environment specifics:
   Using Arduino IDe 1.0.5
   Tested on Redboard, 3.3v/8MHz and 5v/16MHz ProMini hardware.

 ******************************************************************************/

// Define hardware connections
#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 13
#define PIN_ANALOG_IN A0

const int ledPin =  9;                     // define the the LED connection pin
const int brightnessLocked = 10;           // the brightness of the lED when its locked
const int brightnessUnlocked = 255;        // the brightness of the lED when its unlocked
int readingOffset = -1;
// when LEDs are added to the circuit, the reading of the sensor seemed to be influence somehow 
// and all the values are generally raised by a certain amount.
// the offset variable is to make sure the readings are still at the same level as the passcode.

char songCode[1000];                       // the code of the song that is currently playing
char passcode[] = "BBCBAA";                // a repetitive snippet from the song we chose
boolean locked = true;                     // the state of the lock. Whether locked or unlocked. Initially locked

int i;                                     // the variable is used to control the pace of things happening in the loop function



// soundISR()
// This function is installed as an interrupt service routine for the pin
// change interrupt.  When digital input 2 changes state, this routine
// is called.
// It queries the state of that pin, and sets the onboard LED to reflect that
// pin's state.

void soundISR()
{
  int pin_val;

  pin_val = digitalRead(PIN_GATE_IN);
  digitalWrite(PIN_LED_OUT, pin_val);
}

void setup()
{
  Serial.begin(9600);

  //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // configure input to interrupt
  pinMode(PIN_GATE_IN, INPUT);
  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  // Display status
  //  Serial.println("Initialized");

  analogWrite(ledPin, brightnessLocked);             // set the initial brightness of the LED pin. Locked mode

}

void loop()
{
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);

  // Convert envelope value into a message
  Serial.print("Status: ");
  if ((value + readingOffset) <= 10)                // refer to the ISSUES part at the beginning for the reason why a readingOffset is used
  {
    Serial.println("||");
    strcat(songCode, "A");
  }
  else if ( ((value + readingOffset) > 10) && ( (value + readingOffset) <= 30))
  {
    Serial.println("||||");
    strcat(songCode, "B");
  }
  else if ((value + readingOffset) > 30)
  {
    Serial.println("||||||");
    strcat(songCode, "C");
  }
  // contatenating strings in C:
  // https://www.programmingsimplified.com/c-program-concatenate-strings



  if (i % 25 == 0) {
    // statements within these curly brackets gets excuted less often than the loop runs for readability

    // report the current song code after a set interval for debugging use
    Serial.print("songcode: ");
    Serial.println(songCode);


    /* search for passcode in the songCode: */
    char *pointer = strstr(songCode, passcode);  //a variable used to detect if a string is with in a nother
    // https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/search-within-a-string
    // what is a char*:  https://stackoverflow.com/questions/6823249/what-is-a-char

    if (pointer != NULL) {       // if the passcode is found within the current code string, i.e. unlocked
      locked = false;            // update the lock status
      Serial.println("------------you just unlocked!!!------------");    // let me know during which part it is unlocked

    } else {}                    // if the passcode is NOT found within the current code string, i.e. locked

    

    if(locked == true){          // keep reminding me of the current lock state 
      Serial.println("------------locked------------");
    } else {
      Serial.println("------------unlocked------------");
    }

    songCode[0] = 0;            // reset the songCode at the end of every loop to prevent memory overflow
  }


  Serial.println(value);        // print the reading for debug purpose


  i++;                         // increase the timer trigger variable

  ledControl(locked);
  delay(100);                  // pause for 100 millisec before next loop

  //  int b = analogRead(ledPin);
  //  Serial.println(b);           // read the current brightness for debug purpose

}



/*---------------------------------------- Functions ----------------------------------------*/

void ledControl(boolean locked){
  // a function to control the behaviour of the LEDs
  
  int LED = digitalRead(PIN_GATE_IN);           // link the behaviour of the exteranl LEDs to the intertal LED on the sonsor, so that they follow the beats


  if (locked == true) {                         // in the case the program is still locked
    analogWrite(ledPin, brightnessLocked);      // keep the brightness at the unlocked level
  } else {                                      // in the case the program is unlocked
    if(LED == HIGH){
      analogWrite(ledPin, brightnessUnlocked);  // when there's a beat, LED turns on 
    } else {
      analogWrite(ledPin, 0);                   // when there's no beats, LED turns off 
    }
  }

}
