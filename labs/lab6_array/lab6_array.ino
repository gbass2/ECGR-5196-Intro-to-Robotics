/*
* Class:       ECGR 4161/5196 Lab 6
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        02-28-2022
* Description:
*/

#include "SimpleRSLK.h"
#include "HelperRSLK.h"

void setup() {
    setupRSLK();  // Set up all of the pins & functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.begin(38400);
    while(!Serial);
}

void loop() {
    uint8_t wheelSpeed = 15;
    uint16_t degrees = 90; // Degrees to turnInPlace after driving straight.
    uint16_t distance100CM = 100; // Distance to drive 100cm.
    size_t arrSize = 15;
    float distToWall[15];
    float distToWallSorted[15];
    uint16_t perpendicularDist;

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

    // Measure the distances to the wall.
    for(uint8_t i=0; i<arrSize; i++) {
      distToWall[i] = measureDistance();
      turnInPlaceStatic(LEFT);
      delay(1000);
    }

    // Copy the contents of array with the measurements.
    for(uint8_t i=0; i<arrSize; i++) {
      distToWallSorted[i] = distToWall[i]; 
    }

    // Sort the measurements.
    sortArray(distToWallSorted, arrSize);

    // Loop through the array to figure out how much to turn back by.
    uint8_t counter; // Counter to hold how much to turn.
    for(uint8_t i=0; i<arrSize; i++) {
      if(distToWallSorted[0] == distToWall[i]) {
        counter = i;
        break;
      }
    }
    // Turn back to perpendicular.
    for(uint8_t i=0; i<(arrSize-counter); i++) {
      turnInPlaceStatic(RIGHT);
      delay(1000);
    }

    // Drive to wall.
    perpendicularDist = distToWallSorted[0];
    uint16_t distanceToTravel = perpendicularDist - 30; // Subtracting 30cm from the distance to the wall.
    driveStraight(distanceToTravel, FORWARD, wheelSpeed);

    // Turn 90 CW.
    delay(1000);
    turnInPlace(degrees,RIGHT);

    // Drive 100cm.
    delay(1000);
    driveStraight(distance100CM,FORWARD, wheelSpeed);
}
