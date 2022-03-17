/*
* Class:       ECGR 4161/5196 Lab 5
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        02-28-2022
* Description:
*/

#include "SimpleRSLK.h"
#include "HelperRSLK.h"

void setup() {
 setupRSLK();  // Set up all of the pins & functions needed to be used by the TI bot
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 Serial.begin(38400);
 while(!Serial);
}

void loop() {
    uint8_t wheelSpeed = 15;
    uint16_t degrees = 5; // Degrees to turnInPlace
    float currentDistance; // Current distance to wall.
    float prevDistance; // previous distance to wall.
    uint16_t distance100CM = 100; // Distance to drive 100cm.

    // Getting an intial distance.
    currentDistance = measureDistance();

    // Looping and measuring every 5 degrees.
    while(1) {
      prevDistance = currentDistance;
      turnInPlace(degrees, LEFT);
      currentDistance = measureDistance();

      // Checking to see if the current distance is greater than the previous
      // If it is then end the loop
      if((currentDistance >= prevDistance) && (currentDistance < 600))
        break;

      delay(500);
    }

    // Turn back to the right by 5 degrees since current distance was
    // greater than previous distance.
    turnInPlace(degrees,RIGHT);
    currentDistance = prevDistance; // Setting the current distance back to the correct distance from the wall.

    // Drive to wall.
    uint16_t distanceToTravel = currentDistance - 30; // Subtracting 30cm from the distance to the wall.
    driveStraight(distanceToTravel, FORWARD, wheelSpeed);

    // Turn 90 CW.
    delay(1000);
    degrees = 90;
    turnInPlace(degrees,RIGHT);

    // Drive 100cm.
    delay(1000);
    driveStraight(distance100CM,FORWARD, wheelSpeed);

    exit(1);
}
