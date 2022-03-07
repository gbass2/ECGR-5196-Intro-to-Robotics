/*
 * Class:       ECGR 4161/5196
 * Authors:     Grayson Bass, Ryan Roscrow
 * Description: Defines function that will be used over the course of the Class
*/
#ifndef HELPER_RSLK
#define HELPER_RSLK

#include <stdint.h>
#include <math.h>
#include "SimpleRSLK.h"

#define CNT_PER_REV 360
#define WHEEL_DIAMETER 6.99999
#define WHEEL_BASE 14
#define FULL_CIRCLE_DEGREES 360
#define LEFT 0
#define RIGHT 1
#define FORWARD 0
#define BACKWARD 1


uint32_t countForDistance(uint32_t distance);
void driveStraight(uint32_t distance, bool direction, uint8_t wheelSpeed); // Function for driving straight x number of cm.
void driveCircle(uint16_t degrees, uint16_t radius, bool direction); // Function for driving in a circle x number of degrees.
void pivot(uint16_t degrees, bool direction); // Function for pivoting x number of degrees.
void turnInPlace(uint16_t degrees, bool direction); // Function for turning in place x number of degrees.

#endif
