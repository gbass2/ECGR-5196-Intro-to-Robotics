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
    float currentDistance; // Current distance to wall.
    float prevDistance; // previous distance to wall.
    uint16_t distance100CM = 100; // Distance to drive 100cm.

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

    // Getting an intial distance.
    currentDistance = measureDistance();

    // Looping and measuring every 5 degrees.
    while(1) {
      prevDistance = currentDistance;
      turnInPlaceStatic(LEFT);
      currentDistance = measureDistance();

      // Checking to see if the current distance is greater than the previous
      // If it is then end the loop
      if((currentDistance > (prevDistance+0.3)) && (currentDistance < 600)){
        break;
      }

      delay(1000);
    }

    // Turn back to the right by 5 degrees since current distance was
    // greater than previous distance.
    delay(1000);
    turnInPlaceStatic(RIGHT);
//    delay(1000);
//    turnInPlaceStatic(RIGHT);
    currentDistance = prevDistance; // Setting the current distance back to the correct distance from the wall.
    delay(1000);

    // Drive to wall.
    uint16_t distanceToTravel = currentDistance - 30; // Subtracting 30cm from the distance to the wall.
    driveStraight(distanceToTravel, FORWARD, wheelSpeed);

    // Turn 90 CW.
    delay(1000);
    turnInPlace(degrees,RIGHT);

    // Drive 100cm.
    delay(1000);
    driveStraight(distance100CM,FORWARD, wheelSpeed);

    exit(1);
}
