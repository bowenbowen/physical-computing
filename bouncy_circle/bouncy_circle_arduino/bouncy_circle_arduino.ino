/* BOUNCY CIRCLE */
/* created on Oct 6 2019 */
/* by Samantha Brooks, Xiaofang Fan and Bowen Shen */
/* Your hand movement can be detected within a 3d space made by aluminum and cardboard with arduino. 
   The distances to different plates are visualised in Processing. */
   
   
/* REFRENCES: */
/* A Touchless 3D Tracking Interface (the cube) tutorial: https://makezine.com/projects/a-touchless-3d-tracking-interface/ */
/* The original implementation by Kyle McDonald: http://www.instructables.com/id/DIY-3D-Controller/ */
/* The pulse circle: https://www.openprocessing.org/sketch/398438 */

#define resolution 8
#define mains 60 // 60: north america, japan; 50: most other places

#define refresh 2 * 1000000 / mains

const int leftPin = 8;
const int bottomPin = 9;
const int rightPin = 10;

void setup() {
  Serial.begin(115200);

  // unused pins are fairly insignificant,
  // but pulled low to reduce unknown variables
  for(int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  for(int i = 8; i < 11; i++)
    pinMode(i, INPUT);

  startTimer();
}

void loop() {  
  Serial.print(time(leftPin, B00000001), DEC);    // left
  Serial.print(",");
  Serial.print(time(bottomPin, B00000010), DEC);   // bottom
  Serial.print(",");
  Serial.println(time(rightPin, B00000100), DEC);  // right

} 

long time(int pin, byte mask) {
  unsigned long count = 0, total = 0;
  while(checkTimer() < refresh) {
    // pinMode is about 6 times slower than assigning
    // DDRB directly, but that pause is important
    pinMode(pin, OUTPUT);
    PORTB = 0;
    pinMode(pin, INPUT);
    while((PINB & mask) == 0)
      count++;
    total++;
  }
  startTimer();
  return (count << resolution) / total;
}

extern volatile unsigned long timer0_overflow_count;

void startTimer() {
  timer0_overflow_count = 0;
  TCNT0 = 0;
}

unsigned long checkTimer() {
  return ((timer0_overflow_count << 8) + TCNT0) << 2;
}
