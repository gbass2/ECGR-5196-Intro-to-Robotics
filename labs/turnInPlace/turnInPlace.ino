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
 while(!Serial);
 setupRSLK();  // Set up all o f the pins & functions needed to be used by the TI bot
}

void loop() {
  uint16_t degrees = 5;

  delay(2000);
 
//  // Turn 90 degrees CCW.
//  turnInPlace(degrees,LEFT);
//  delay(1000);

  for(uint8_t i=0; i < 36; i++){
    // Turn 90 degrees CW.
    turnInPlace(degrees,RIGHT);
    Serial.println(i);
    delay(500);
  }


  exit(1);
}
