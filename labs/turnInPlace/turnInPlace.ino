/*
* Class:       ECGR 4161/5196 Lab 5
* Authors:      Grayson Bass, Ryan Roscrow
* Date:        02-28-2022
* Description: Turn in place.
*/

#include "SimpleRSLK.h"
#include "HelperRSLK.h"

void setup() {
 Serial.begin(38400);
 setupRSLK();  // Set up all o f the pins & functions needed to be used by the TI bot
}

void loop() {
  uint16_t degrees = 90;

  delay(2000);
 
//  // Turn 90 degrees CCW.
//  turnInPlace(degrees,LEFT);
//  delay(1000);

  // Turn 90 degrees CW.
  turnInPlace(degrees,RIGHT);
  delay(1000);


  exit(1);
}
