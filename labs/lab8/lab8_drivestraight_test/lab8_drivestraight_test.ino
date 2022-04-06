/*
* Class:       ECGR 4161/5196 
* Authors:      Grayson Bass, Ryan Roscrow
* Date:        
* Description:
*/

#include "HelperRSLK.h"

void setup() {
 setupRSLK();  // Set up all o f the pins & functions needed to be used by the TI bot
}

void loop() {
  uint8_t degrees = 180;
  uint16_t distance = 25; // cm.
  uint8_t wheelSpeed = 15;

  delay(3000);

  // Drive 5m.
  for(uint8_t i=0; i < 20; i++){
    driveStraight(distance,FORWARD,wheelSpeed);
    delay(500);
  }

  // Turn 180
  turnInPlace(degrees,RIGHT);

  // Drive 5m.
  for(uint8_t i=0; i < 20; i++){
    driveStraight(distance,FORWARD,wheelSpeed);
    delay(500);
  }
  exit(1);
}
