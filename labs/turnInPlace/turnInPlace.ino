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

  uint16_t degrees = 180;
  delay(3000);
  turnInPlace(degrees,RIGHT);

}

void loop() {
}
