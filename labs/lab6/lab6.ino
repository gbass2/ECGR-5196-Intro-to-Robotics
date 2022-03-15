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
    uint16_t degrees = 90; // Degrees to turnInPlace
    uint16_t distanceToWall; // Current distance to wall.    
}
