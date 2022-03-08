/*
* Class:       ECGR 4161/5196 Lab 5
* Authors:      Grayson Bass, Ryan Roscrow
* Date:        02-28-2022
* Description: Drive straight along a given radius of a circle.
*              Turn 90 degrees CCW.
*              Drive 360 degrees from the end of the radius.
*              Turn 90 degrees CCW.
*              Drive along the radius of the circle back to the starting position.
*/

#include "SimpleRSLK.h"
#include "HelperRSLK.h"

void printPin() {
  Serial.println("pressed");
}

void setup() {
 setupRSLK();  // Set up all of the pins & functions needed to be used by the TI bot
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 Serial.begin(38400);
 while(!Serial);
}

void loop() {
  Serial.println(measureDistance());
  delay(1000);
}
