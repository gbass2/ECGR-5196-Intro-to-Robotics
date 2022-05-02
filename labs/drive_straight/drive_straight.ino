/*
* Class:       ECGR 4161/5196 
* Authors:      Grayson Bass, Ryan Roscrow
* Date:        
* Description:
*/

#include "HelperRSLK.h"

void setup() {
 setupRSLK();  // Set up all o f the pins & functions needed to be used by the TI bot
 Serial.begin(38400);
 while(!Serial);

 resetLeftEncoderCnt();
 resetRightEncoderCnt();
}

void loop() {
  uint8_t degrees = 180;
  uint16_t distance = 100; // cm.
  uint8_t wheelSpeed = 30;

  delay(5000);
////
  driveStraight(distance,FORWARD,wheelSpeed);

  exit(1);

  // Drive 5m.
  for(uint8_t i=0; i < 15; i++){
    driveStraight(distance,FORWARD,wheelSpeed);
  }
  exit(1);
  // Turn 180
  turnInPlace(degrees,RIGHT);

  // Drive 5m.
  for(uint8_t i=0; i < 10; i++){
    driveStraight(distance,FORWARD,wheelSpeed);
    delay(500);
  }
  exit(1);
}
