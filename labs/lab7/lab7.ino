/*
* Class:       ECGR 4161/5196 Lab 7
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        04-04-2022
* Description: Use servo motor and ultrasonic sensor to identify closest object
*              then drive to it.
*/

#include <Servo.h>
#include "HelperRSLK.h"
Servo myservo;  // create servo object to control a servo

void setup() {
    setupRSLK();  // Set up all of the pins &
                  // functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.begin(38400);
    while(!Serial);

    myservo.attach(38);  // attaches the servo on Port 2.4 to the servo object
    myservo.write(0);    // Send it to the default position

   attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_0),stopMotorInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_1),stopMotorInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_2),stopMotorInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_3),stopMotorInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_4),stopMotorInterrupt,RISING);
   attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_5),stopMotorInterrupt,RISING);
}

void loop() {
    uint8_t wheelSpeed = 15;
    uint16_t degrees = 180; // Degrees to turnInPlace after driving straight.
    float closestDistance; // Distance to the closest object
    size_t arrSize = 72; // Size of the array to store the distances.
    float distToWall[72]; // Array to store the unsorted distances.
    float distToWallSorted[72]; // Array to store the sorted distances.

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

    uint8_t j = 0;

    // Do the first sweep and measure distances.
    for(uint8_t pos = 0; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      distToWall[j] = measureDistance();
      delay(450);                       // waits 450ms for the servo to reach the position

      j++;
    }

    // Turn in place 180 degrees.
    turnInPlace(degrees, RIGHT);

    // Do the second sweep and measure distances.
    for(uint8_t pos = 0; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      distToWall[j] = measureDistance();
      delay(450);                       // waits 450ms for the servo to reach the position

      j++;
    }

    // Copy the contents of array with the measurements.
    for(uint8_t i=0; i<arrSize; i++) {
      distToWallSorted[i] = distToWall[i];
    }

    // Sort the measurements.
    sortArray(distToWallSorted, arrSize);
    closestDistance = distToWallSorted[0];

    // Loop through the array to figure out how much to turn back to the object.
    uint8_t counter; // Counter to hold how much to turn.
    for(uint8_t i=0; i<arrSize; i++) {
      if(distToWallSorted[0] == distToWall[i]) {
        counter = i;
        break;
      }
    }

    delay(3000);

    // Calculate the degrees to turn towards object and turn.
    degrees = 5;
    uint16_t degreesToTurn = degrees*(counter) + 90;
    turnInPlace(degreesToTurn, RIGHT);


    // Drive to object.
    driveStraight(closestDistance, FORWARD, wheelSpeed);
}
