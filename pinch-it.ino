
/****************************************ISSUE TO BE SOLVED****************************************/
/* the update frequency of the buzzer and the LED can not be separated. 
   I want the buzzer to update every few hundred millisec so that it sounds like a 8-bit tune.
   On the other hand, I want the LED to refresh as frequent as possible so that the transition between different brightnesses are smooth.
   If I run both functions in the loop function, a delay in one of the functions affect the update frequency of all other parts.
   
   One solution I can think of for now is to use a for loop + modulo to limit the pace of the buzzer. 
   However, I don't know if a for loop can run forever. Otherwise there's be a time limit for the program.*/

    
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ISSUE SOLVED^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/* Solved by adding a global variable called updateFrenquencyLimiter. 
   Use it in the loop function (basically a loop that runs forever, which is what I was looking for) with modulo.
   So now the LED is updating at 5 times the speed than the buzzer.*/


  
/*--------------------------------------------- MAIN ---------------------------------------------*/

//globle variables
int buzzerPin = 9;
int ledPin = 10;
int sensorPin = A0;

int sensorReading;
int sensorReadingMin = 0;
int sensorReadingMax = 1000;

int updateFrenquencyLimiter = 0;                         // a variable used to limit the update frequency of the buzzer, so that it updates at a lower pace then the LED (refer to the documented issue solution)


void setup() {
  
  
  Serial.begin(9600);                                    // initialize serial communications (for debugging only):
  
  pinMode(ledPin, OUTPUT);                               // set the pin for the led
  
}


void loop() {

  sensorReading = analogRead(sensorPin);                // read the sensor
  // sensorReading = map(sensorReading, 1,1024, 1,1000);

  Serial.print("sensorReading: ");
  Serial.println(sensorReading);                       // print the sensor reading so you know its range

  if (updateFrenquencyLimiter%3 == 0){
    buzz(sensorReading);                               // the buzzer buzzes once every 3  times the LED refreshes
  }
  led(sensorReading);                                  // light the LED 

  Serial.println("--------------------------");        // print a divider for readability
  
  updateFrenquencyLimiter++;

  delay(10);                                           // delay in between reads for stability

}


/*--------------------------------------------- FUNCTIONS ---------------------------------------------*/


void buzz(int sensorReading) {

  // a function that controls the behaviour of the buzzer
  // map the analog input range (in this case, 0 - 1000 from the pressure sensor)
  // to the output pitch range (120 - 1000Hz)

  int lowPitch = 120;             // the lowest pitch
  int highPitch = 1000;           // the hightest pitch
  int thisPitch = map(sensorReading, sensorReadingMin, sensorReadingMax, lowPitch, highPitch);     // calculate the current pitch value according to the sensor reading

  Serial.print("thisPitch: ");
  Serial.println(thisPitch);      // print the current pitch for debugging

  tone(buzzerPin, thisPitch, 30); // set the buzzer's pin, play the pitch

//  delay(300);                   // delay in between reads for stability

}


/*--------------------------------------------------------*/



void led(int sensorReading) {

  // a function that controls the behaviour of the LED
  // make LED brighter according to sensor

  //  int shortInterval = 200;
  //  int longInterval = 2000;
  //  int blinkInterval = map(sensorReading, 0, 1000, shortInterval, longInterval);



  int baseBrightness = 10;                             // how bright the LED initially is (0-255)
  int fadeAmount = map(sensorReading, sensorReadingMin, sensorReadingMax, 0, (255-baseBrightness));  // calculate how much the brightness changes according to the sonsor reading
  int thisBrightness = baseBrightness + fadeAmount;    // change the current brightness

  Serial.print("thisBrightness: ");
  Serial.println(thisBrightness);                      // print the current brightness for debugging

  // the syntax for analogWrite: https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
  analogWrite(ledPin, thisBrightness);                 // set the initial brightness of pin 9:
  
//  delay(30);                                           // wait for 30 milliseconds to see the dimming effect

}
