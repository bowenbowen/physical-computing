// CUBEVERSE
// Oct 2019
// A cube that responds movements by changing light pattern
// Team: Xiaofang Fan, Hyelim Lim, Yuan Chen, Bowen Shen


#include <Math.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    6
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 16
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// the x y z values when the cube is upright flat
int xInitial = 317;
int yInitial = 324;
int zInitial = 397;

// the x y z values when the cube is up-side-down flat
int xInitialFlipped = 317;
int yInitialFlipped = 322;
int zInitialFlipped = 267;

// the value of z when its lying on the side surface
int zMid = 320;
// can be used to detect if the cube is upright or flipped
// note that this is not equal to the average of z upright (max) and z flipped (min)

int tolerance = 10;
// ^ the readings can be unstable.
// ^ the hardware will only response if the value difference passes beyond the fluctuation tolerane range

int bottoms[] = { 4 - 1, 5 - 1, 9 - 1, 16 - 1 };
int lefts[] = { 4 - 1, 8 - 1, 9 - 1, 13 - 1 };
int tops[] = { 1 - 1, 8 - 1, 12 - 1, 13 - 1 };
/* note: the first LED has the index of 0. */
/* in order to match the natural count, all the indexes of LED are indicated as (n-1) in this code  */
/* where n indicates the natural count index */



void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP

  calibration();
}

void loop() {

  // read the input on analog pin 0:
  int x = analogRead(A0);
  int y = analogRead(A1);
  int z = analogRead(A2);

  // print out the reading from the accelerometer:
  Serial.print("x: ");
  Serial.print(x, DEC);
  Serial.print(", y: ");
  Serial.print(y, DEC);
  Serial.print(", z: ");
  Serial.println(z, DEC);

  orientDetect(x, y, z);

  delay(100);        // delay in between reads for stability
}



/* -------------------------------------------------------------------------------------------*/
/* ---------------------------------------- FUNCTIONS ----------------------------------------*/
/* -------------------------------------------------------------------------------------------*/

void calibration() {
  // the initial values are based on readings when the sensor works on 3.3V while connected to laptop power
  // when using under the same conditions calibration is not necessary, as the difference of baseline values are minor
  // however, calibration is recommended when operationg under different conditions, eg on battery power

  ///////////////////////////////
  // Calibration instructions: //
  ///////////////////////////////
  // let the cube sit still and flat on the bottom when connected to power.
  // when the LEDs on the bottom turn green, that means calib for flat state is done (which can happen quickly, ~1-2 sec)
  // turn the cube up-side-down within 5 seconds after the bottom calib is done
  // let the cube stay still until the calib is done.
  // the cube will start working automatically.
  
  // variables for calibration
  int sampleNum = 20;  // how many samples will be taken
//  int zMidSamples[sampleNum];
  int xTotal = 0;
  int yTotal = 0;
  int zTotal = 0;
//  int zMidTotal = 0;
  int index = 0; 
  boolean flatCalib = false;
//  boolean sideCalib = false;
  boolean flipCalib = false;

  // flat state calibration
  while (!flatCalib) {
    strip.clear();

    ////////////////////////// waiting for calib effect //////////////////////////
    Serial.println("Calibrating flat state...Please wait");
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(bottoms[i], strip.Color(200, 0, 0));
      strip.show();
    }
    
    while ( index < sampleNum ) {          // add the current reading to the sample array
      xTotal += analogRead(A0);
      yTotal += analogRead(A1);
      zTotal +=  analogRead(A2);
      index++;
    }

    xInitial = round(xTotal / sampleNum);  // get averages
    yInitial = round(yTotal / sampleNum);
    zInitial = round(zTotal / sampleNum);
    Serial.println("Calibration done.");
    Serial.print("Calibrated flat state: ");
    Serial.print(" x: ");
    Serial.print(xInitial);
    Serial.print(", y: ");
    Serial.print(yInitial);
    Serial.print(", z: ");
    Serial.println(zInitial);
    index = 0;                            // reset all the variables for next step of calibration
    xTotal = 0;
    yTotal = 0;
    zTotal = 0;
    flatCalib = true;                     // flat calib checked. Can proceed

    ////////////////////////// flat calib done effect //////////////////////////
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(bottoms[i], strip.Color(0, 200, 0));
      strip.show();
    }
    
    Serial.println("Please get ready to calibrate flipped state.");
  }


  // flipped state calibration
  while (!flipCalib) {
    ////////////////////////// waiting for calib effect //////////////////////////
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(tops[i], strip.Color(200, 0, 0));
      strip.show();
    }

    delay(5000);        // leave some time to put the cube to the right postition
    
    Serial.println("Calibrating flipped state...Please wait");

    while ( index < sampleNum ) {          // add the current reading to the sample array
      xTotal += analogRead(A0);
      yTotal += analogRead(A1);
      zTotal +=  analogRead(A2);
      index++;
    }

    xInitialFlipped = round(xTotal / sampleNum);  // get averages
    yInitialFlipped = round(yTotal / sampleNum);
    zInitialFlipped = round(zTotal / sampleNum);
    Serial.println("Calibration done.");
    Serial.print("Calibrated flipped state: ");
    Serial.print(" x: ");
    Serial.print(xInitialFlipped);
    Serial.print(", y: ");
    Serial.print(yInitialFlipped);
    Serial.print(", z: ");
    Serial.println(zInitialFlipped);
    index = 0;

    flipCalib = true;                     // flip calib checked. Can proceed
  }
  ////////////////////////// flip calib done effect //////////////////////////
  for (int i = 0; i < 4; i++) {
    strip.setPixelColor(tops[i], strip.Color(0, 200, 0));
    strip.show();
  }
  delay(1000);                            // minor delay for the effect to be seen
}



/* -------------------------------------------------------------------------------------------*/

boolean upright = true;   // when upright, = true; when flipped, = false
char *l_r = "flat";       // when left, = "left"; when right, = "right"; when flat, = "flat"
char *f_b = "flat";       // when front, = "front"; when back, = "back"; when flat, = "flat"

// a function to detect current orientation of the cube
void orientDetect(int xVal, int yVal, int zVal) {

  // verticle state detection:
  if (zVal > ( zMid + tolerance )) {
    upright = true;
  } else if (zVal < ( zMid - tolerance )) {
    upright = false;
  }
  Serial.print(upright ? "upright " : "flipped "); // print verticle state 


  // horizontal state detection:
  if (upright) {
    if ( yVal < yInitial - tolerance ) {         // -------------- tilting right on x-axis
      l_r = "right";
      if ( xVal > xInitial + tolerance ) {       // upright right front
        f_b = "front";
      }
      else if ( xVal < xInitial - tolerance ) {  // upright right back
        f_b = "back";
      }
      else {                                     // upright right right
        f_b = "flat";
      }
    } else if ( yVal > yInitial + tolerance ) {  // -------------- tilting left on x-axis
      l_r = "left";
      if ( xVal > xInitial + tolerance ) {       // upright left front
        f_b = "front";
      }
      else if ( xVal < xInitial - tolerance ) {  // upright left back
        f_b = "back";
      }
      else {                                     // upright left left
        f_b = "flat";
      }
    } else {                                     // -------------- not tilting, flat on x-axis
      l_r = "flat";
      if ( xVal > xInitial + tolerance ) {       // upright flat front
        f_b = "front";
      }
      else if ( xVal < xInitial - tolerance ) {  // upright flat back
        f_b = "back";
      }
      else {                                     // upright flat flat
        f_b = "flat";
      }
    }
  } else if (!upright) {
    if ( yVal < yInitialFlipped - tolerance ) {         // -------------- tilting left on x-axis
      l_r = "left";
      if ( xVal > xInitialFlipped + tolerance ) {       // flipped left front
        f_b = "front";
      }
      else if ( xVal < xInitialFlipped - tolerance ) {  // flipped left back
        f_b = "back";
      }
      else {                                            // flipped left flat
        f_b = "flat";
      }
    } else if ( yVal > yInitialFlipped + tolerance ) {  // -------------- tilting right on x-axis
      l_r = "right";
      if ( xVal > xInitialFlipped + tolerance ) {       // flipped right front
        f_b = "front";
      }
      else if ( xVal < xInitialFlipped - tolerance ) {  // flipped right back
        f_b = "back";
      }
      else {                                            // flipped right flat
        f_b = "flat";
      }
    } else {                                            // -------------- not tilting, flat on x-axis
      l_r = "flat";
      if ( xVal > xInitialFlipped + tolerance ) {       // flipped flat front
        f_b = "front";
      }
      else if ( xVal < xInitialFlipped - tolerance ) {  // flipped flat back
        f_b = "back";
      }
      else {                                            // flipped flat flat
        f_b = "flat";
      }
    }

  }

  // print the current horizontal state
  Serial.print(l_r);
  Serial.print(" ");
  Serial.print(f_b);
  Serial.println(" ");

  // call the function that makes the LEDs react to orientation changes
  ledReact(upright, l_r, f_b);
}





/* -------------------------------------------------------------------------------------------*/

void ledReact( int upright, char *l_r, char *f_b ) { // pass amltiple characters with *
  strip.clear(); // Set all pixel colors to 'off'
  int col = 150; // brightness of the LED, hence we decide only to use white light

  if (upright) {
    if ( l_r == "right" ) {         // -------------- tilting right on x-axis
      if ( f_b == "front" ) {       // upright right front
        strip.setPixelColor((5 - 1), strip.Color(col, col, col));
        strip.setPixelColor((6 - 1), strip.Color(col, col, col));
      }
      else if ( f_b == "back" ) {   // upright right back
        strip.setPixelColor((16 - 1), strip.Color(col, col, col));
        strip.setPixelColor((15 - 1), strip.Color(col, col, col));
      }
      else {                        // upright right flat
        strip.setPixelColor((5 - 1), strip.Color(col, col, col));
        strip.setPixelColor((16 - 1), strip.Color(col, col, col));
      }
    } else if ( l_r == "left" ) {   // -------------- tilting left on x-axis
      if ( f_b == "front" ) {       // upright left front
        strip.setPixelColor((4 - 1), strip.Color(col, col, col));
        strip.setPixelColor((3 - 1), strip.Color(col, col, col));
      }
      else if ( f_b == "back" ) {   // upright left back
        strip.setPixelColor((9 - 1), strip.Color(col, col, col));
        strip.setPixelColor((10 - 1), strip.Color(col, col, col));
      }
      else {                        // upright left flat
        strip.setPixelColor((4 - 1), strip.Color(col, col, col));
        strip.setPixelColor((9 - 1), strip.Color(col, col, col));
      }
    } else {                        // -------------- not tilting, flat on x-axis
      if ( f_b == "front" ) {       // upright flat front
        strip.setPixelColor((4 - 1), strip.Color(col, col, col));
        strip.setPixelColor((5 - 1), strip.Color(col, col, col));
      }
      else if ( f_b == "back" ) {   // upright flat back
        strip.setPixelColor((9 - 1), strip.Color(col, col, col));
        strip.setPixelColor((16 - 1), strip.Color(col, col, col));
      }
      else {                        // upright flat flat
        flatState(col / 5);
      }
    }
  } else if (!upright) {
    if ( l_r == "left" ) {          // -------------- tilting left on x-axis
      if ( f_b == "front" ) {       // flipped left front
        strip.setPixelColor((11 - 1), strip.Color(col, col, col));
        strip.setPixelColor((12 - 1), strip.Color(col, col, col));
      }
      else if ( f_b == "back" ) {   // flipped left back
        strip.setPixelColor((1 - 1), strip.Color(col, col, col));
        strip.setPixelColor((2 - 1), strip.Color(col, col, col));
      }
      else {                        // flipped left flat
        strip.setPixelColor((5 - 1), strip.Color(col, col, col));
        strip.setPixelColor((12 - 1), strip.Color(col, col, col));
      }
    } else if ( l_r == "right" ) {  // -------------- tilting right on x-axis
      if ( f_b == "front" ) {       // flipped right front
        strip.setPixelColor((13 - 1), strip.Color(col, col, col));
        strip.setPixelColor((14 - 1), strip.Color(col, col, col));
      }
      else if ( f_b == "back" ) {   // flipped right back
        strip.setPixelColor((7 - 1), strip.Color(col, col, col));
        strip.setPixelColor((8 - 1), strip.Color(col, col, col));
      }
      else {                        // flipped right flat
        strip.setPixelColor((13 - 1), strip.Color(col, col, col));
        strip.setPixelColor((8 - 1), strip.Color(col, col, col));
      }
    } else {                        // -------------- not tilting, flat on x-axis
      if ( f_b == "front" ) {       // flipped flat front
        strip.setPixelColor((12 - 1), strip.Color(col, col, col));
        strip.setPixelColor((13 - 1), strip.Color(col, col, col));
      }
      else if ( f_b == "back" ) {    // flipped flat back
        strip.setPixelColor((1 - 1), strip.Color(col, col, col));
        strip.setPixelColor((8 - 1), strip.Color(col, col, col));
      }
      else {                         // flipped flat flat
        flatState(col / 5);
      }
    }
  }
  strip.show();
}

/* -------------------------------------------------------------------------------------------*/

void flatState(int col) {
  // when flat, the central LEDs are on
  strip.setPixelColor((2 - 1), strip.Color(col, col, col));
  strip.setPixelColor((7 - 1), strip.Color(col, col, col));
  strip.setPixelColor((11 - 1), strip.Color(col, col, col));
  strip.setPixelColor((14 - 1), strip.Color(col, col, col));
  strip.setPixelColor((3 - 1), strip.Color(col, col, col));
  strip.setPixelColor((6 - 1), strip.Color(col, col, col));
  strip.setPixelColor((10 - 1), strip.Color(col, col, col));
  strip.setPixelColor((15 - 1), strip.Color(col, col, col));
}
