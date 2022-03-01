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

void setup() {
  Serial.begin(38400);
  setupRSLK();  // Set up all o f the pins & functions needed to be used by the TI bot
}

void loop() {
  uint8_t wheelSpeed = 15;
  uint16_t degrees = 90;
  uint16_t radius = 75; // Radius of circle in cm.

  delay(2000);
  // Drive the robot 75 cm.
  driveStraight(radius,FORWARD,wheelSpeed);
  delay(5000);

  // Turn 90 degrees CCW.
  turnInPlace(degrees,LEFT);
  delay(1000);

  // Drive in a circle with radius 75 cm.
  degrees = 360;
  driveCircle(degrees,radius,LEFT);
  delay(8000 );

  // Turn 90 degrees CCW
  degrees = 90;
  turnInPlace(degrees,LEFT);
  delay(1000);

  // Drive the robot 75 cm.
  driveStraight(straight,FORWARD,wheelSpeed);

  exit(1);
}
