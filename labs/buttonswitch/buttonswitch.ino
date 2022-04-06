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

void stopMotor() {
  disableMotor(BOTH_MOTORS);
}

void setup() {
 setupRSLK();  // Set up all of the pins & functions needed to be used by the TI bot

 attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_0),stopMotorInterrupt,RISING);
 attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_1),stopMotorInterrupt,RISING);
 attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_2),stopMotorInterrupt,RISING);
 attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_3),stopMotorInterrupt,RISING);
 attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_4),stopMotorInterrupt,RISING);
 attachInterrupt(digitalPinToInterrupt(BP_SW_PIN_5),stopMotorInterrupt,RISING); 

 Serial.begin(38400);
 while(!Serial);
}

void loop() {
  uint16_t distance = 1000; //cm
  uint8_t wheelSpeed = 15;
  driveStraight(distance,FORWARD,wheelSpeed);
  exit(1);
}
