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

const int trigPin = 32;  // This is Port Pin 3.5 on the MSP432 Launchpad.
const int echoPin = 33; // This is Port Pin 5.1 on the MSP432 Launchpad.

// Functions for driving the robot.
uint32_t countForDistance(float distance);
void driveStraight(float distance, bool direction, uint8_t wheelSpeed); // Function for driving straight x number of cm.
void turnInPlace(uint16_t degrees, bool direction); // Function for turning in place x number of degrees.

// Function that returns the median distance using an ultrasonic sensor
float measureDistance();

/*Sorts an array of any type. Pass array by reference.
  The definition has to be in the header file.
  Parameters:
  array (T): Templated array to be sorted.

  Returns:
  void
*/
template<typename T> void sortArray(T& array, size_t arrSize) {
  float hold;
  for(size_t i=0; i < arrSize; i++) {
    for(size_t j=0; j < arrSize; j++) {
      if(array[j]>array[j+1]) {
        hold=array[j];
        array[j]=array[j+1];
        array[j+1]=hold;
      }
    }
  }
}

#endif
