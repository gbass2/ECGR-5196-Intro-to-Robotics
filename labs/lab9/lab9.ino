/*
* Class:       ECGR 4161/5196 Lab 9
* Authors:     Grayson Bass
* Date:        05-02-2022
* Description: 
*/

#include <Servo.h>
#include "HelperRSLK.h"
Servo myservo;  // create servo object to control a servo


void setup() {
    setupRSLK();  // Set up all of the pins &
                  // functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.

    initFollowLine();
    tofInit();

    Serial.begin(38400);
    while(!Serial);

    myservo.attach(38);  // attaches the servo on Port 2.4 to the servo object
    myservo.write(0);    // Send it to the default position
}

void loop() {
    uint8_t wheelSpeed = 30;
    uint16_t degrees = 180; // Degrees to turnInPlace after driving straight.
    float closestDistance; // Distance to the closest object
    size_t arrSize = 72; // Size of the array to store the distances.
    float distToWall[72]; // Array to store the unsorted distances.
    float distToWallSorted[72]; // Array to store the sorted distances.
    uint8_t botLength = 16.5; // cm. Length of the robot.
    

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

    uint8_t j = 0;

    // Do the first sweep and measure distances.
    for(uint8_t pos = 0; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      distToWall[j] = measureTOFCM();
      delay(100);                       // waits 450ms for the servo to reach the position

      j++;
    }

    // Turn in place 180 degrees.
    turnInPlace(degrees, RIGHT);
    myservo.write(0);
    delay(500);
    // Do the second sweep and measure distances.
    for(uint8_t pos = 0; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      distToWall[j] = measureTOFCM();
      delay(100);                       // waits 450ms for the servo to reach the position

      j++;
    }

    // Copy the contents of array with the measurements.
    for(uint8_t i=0; i<arrSize; i++) {
      distToWallSorted[i] = distToWall[i];
    }

    // Sort the measurements.
    sortArray(distToWallSorted);

   for(uint8_t i=0; i<arrSize; i++) {
      Serial.println("UnSorted Values: " + String(distToWall[i]));
    }
    
    for(uint8_t i=0; i<arrSize; i++) {
      Serial.println("Sorted Values: " + String(distToWallSorted[i]));
    }
    
    closestDistance = distToWallSorted[0];
    
    // Loop through the array to figure out how much to turn back to the object.
    uint8_t counter; // Counter to hold how much to turn.
    Serial.println("closest distance: " + String(closestDistance));
    for(uint8_t i=0; i<arrSize; i++) {
      if(closestDistance == distToWall[i]) {
        counter = i;
        break;
      }
    }
    Serial.println("counter: " + String(counter));

    delay(500);

    // Calculate the degrees to turn the wall and turn.
    degrees = 5;
    uint16_t degreesToTurn = degrees*(counter) + 90;
    turnInPlace(degreesToTurn, RIGHT);

    // Turn servo straight.
    myservo.write(89);
    delay(500);              


    float distanceToCenter[4];
    distanceToCenter[0] = measureTOFCM();
    
    degrees = 90;
    turnInPlace(degrees, RIGHT);
    distanceToCenter[1] = measureTOFCM();
    delay(100);

    turnInPlace(degrees, RIGHT);
    distanceToCenter[2] = measureTOFCM();
    delay(100);

    turnInPlace(degrees, RIGHT);
    distanceToCenter[3] = measureTOFCM();
    delay(100);

    if(distanceToCenter[4] < distanceToCenter[2]) {
      turnInPlace(degrees, LEFT);
    } else {
      turnInPlace(degrees, RIGHT);
    }
    delay(100);
    
    // Drive to the middle of both dstances.
    uint16_t distanceToMiddle = (distanceToCenter[0] + distanceToCenter[1] + botLength)/2;

    driveStraight(distanceToMiddle, FORWARD, wheelSpeed);
    delay(100);

    if(distanceToCenter[4] < distanceToCenter[2]) {
      turnInPlace(degrees, RIGHT);
    } else {
      turnInPlace(degrees, LEFT);
    }
}
