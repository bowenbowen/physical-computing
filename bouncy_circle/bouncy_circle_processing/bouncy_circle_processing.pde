/* BOUNCY CIRCLE */
/* created on Oct 6 2019 */
/* by Samantha Brooks, Xiaofang Fan and Bowen Shen */
/* Your hand movement can be detected within a 3d space made by aluminum and cardboard with arduino. 
   The distances to different plates are visualised in Processing. */
   
   
/* REFRENCES: */
/* A Touchless 3D Tracking Interface (the cube) tutorial: https://makezine.com/projects/a-touchless-3d-tracking-interface/ */
/* The original implementation by Kyle McDonald: http://www.instructables.com/id/DIY-3D-Controller/ */
/* The pulse circle: https://www.openprocessing.org/sketch/398438 */


import processing.serial.*;

float leftValue = 0;        // left value
float bottomValue = 0;      // bottom value
float rightValue = 0;       // right value

float leftMax = 25000;      // close
float leftMin = 19900;      // far
float bottomMax = 35500;    // close
float bottomMin = 29000;    // far
float rightMax = 28000;     // close
float rightMin = 21000;     // far

int bounceMin = 1;          // how bouncy the waves are
int bounceMax = 40;

int stepMin = 20;           // the less the steps, the narrower spacings are
int stepMax = 5;            // hence the more lines there are

Serial serial;

int serialPort = 3;         // the current port in use. Check with Serial.list() in setup


void setup() {
  size(850, 850);
  noFill();
  strokeWeight(2);
  strokeCap(SQUARE);

  printArray(Serial.list());
  serial = new Serial(this, Serial.list()[serialPort], 115200);  
  serial.bufferUntil('\n');
}

void draw() {
  background(238, 243, 239);
  translate(width / 2, height / 2);
  float radius = 200;
  float step = 10;           // line spacing, influencing the number of lines
  int senReadAdj = 1000;     // make the red more sensitive to proximity
  
  for (float y = -radius + step / 2; y <= radius - step / 2; y += step) {

    float weight = map(bottomValue, bottomMin, bottomMax, 0.1, 8);  // make the line weight changes according to the distance to the bottom plate  
    weight = constrain(weight, 0.1, 8);
    float power = round(map(bottomValue, bottomMin, bottomMax, bounceMin, bounceMax));
    power = constrain(power, bounceMin, bounceMax);
    float wave = abs(pow(sin(y * 0.003 + frameCount * 0.1), power));
    float wy = y - map(wave, 0, 1, -step, step);      // current location of the wave
    float X = sqrt(sq(radius) - sq(y)) /*radius*/ * map(wave, 0, 1, 1, 1.1); // the width of a wave
    float cRate = map(y, -radius + step / 2, radius + step / 2, 0, 1);       // colour gradiation
    float leftRate = map(leftValue, leftMin, (leftMax-senReadAdj), 0, 1.0); 
    leftRate = constrain(leftRate, 0, 1.0);
    float rightRate = map(rightValue, rightMin, (rightMax-senReadAdj), 0, 1.0); 
    rightRate = constrain(rightRate, 0, 1.0);
    color leftColor = lerpColor(color(69, 189, 207), color(234, 84, 93), leftRate);   // the colour representing the distance to the left plate
    color rightColor = lerpColor(color(69, 189, 207), color(234, 84, 93), rightRate); // the colour representing the distance to the right plate
    stroke(lerpColor(leftColor, rightColor, cRate));
    // blue: color(69, 189, 207)  
    // red: color(234, 84, 93)

    strokeWeight(weight);

    beginShape();
    for (float x = -X; x <= X; x += 1) {
      vertex(x, wy);
    }
    endShape();
  }
}

void serialEvent(Serial serial) {
  // get the ASCII string:
  String inString = serial.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);

    // split the string on the commas and convert the resulting substrings
    // into an integer array:
    float[] values = float(split(inString, ","));

    // if the array has at least three elements, you know you got the whole
    // thing.  Put the numbers in the value variables:
    if (values.length >= 3) {
      leftValue = values[0];
      bottomValue = values[1];
      rightValue = values[2];
    }
  }
}
