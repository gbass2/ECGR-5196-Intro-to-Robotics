/*
 * Class:       ECGR 4161/5196
 * Authors:     Grayson Bass, Ryan Roscrow
 * Description: Defines function that will be used over the course of the Class
*/
#ifndef HELPER_RSLK
#define HELPER_RSLK

#include <Wire.h>
#include "SparkFun_VL53L1X.h"
#include <stdint.h>
#include <math.h>
#include "SimpleRSLK.h"

#define CNT_PER_REV 360
#define WHEEL_DIAMETER 6.99999
#define WHEEL_BASE 14
#define BOT_LENGTH 16.5 // Length of the bot.
#define BOT_WIDTH 7 // Length of the bot.
#define FULL_BOT_WIDTH 14
#define FULL_CIRCLE_DEGREES 360
#define LEFT 0
#define RIGHT 1
#define FORWARD 0
#define BACKWARD 1
#define RED 75    // Define RED of the tri-color LED as pin 75

const int trigPin = 32;  // This is Port Pin 3.5 on the MSP432 Launchpad.
const int echoPin = 33; // This is Port Pin 5.1 on the MSP432 Launchpad.
extern SFEVL53L1X distanceSensor;
extern uint16_t sensorVal[LS_NUM_SENSORS];
extern uint16_t sensorCalVal[LS_NUM_SENSORS];
extern uint16_t sensorMaxVal[LS_NUM_SENSORS];
extern uint16_t sensorMinVal[LS_NUM_SENSORS];

// Functions for driving the robot.
uint32_t countForDistance(float distance);
void driveStraight(float distance, bool direction, uint8_t wheelSpeed); // Function for driving straight x number of cm.
// void driveCircle(uint16_t degrees, uint16_t radius, bool direction); // Function for driving in a circle x number of degrees.
// void pivot(uint16_t degrees, bool direction); // Function for pivoting x number of degrees.
void turnInPlace(float degrees, bool direction); // Function for turning in place x number of degrees.
void turnInPlaceStatic(uint32_t encoderCount, bool direction); // Function for turning in place a set number of pulses.
// void stopMotorInterrupt(); // Stops the motor using interrupts.

// Function to follow line.
void initFollowLine();
void followLine();

// Function that returns the median distance using an ultrasonic sensor and ToF
// float measureDistanceUltra();
void tofInit();
int measureTOFMM(); // Measure distance with tof in mm.
float measureTOFCM(); // Measure distance with tof in cm.


/*Sorts an array of any type. Pass array by reference.
  The definition has to be in the header file.
  Parameters:
  array (T): Templated array to be sorted.

  Returns:
  void
*/

template<typename T, size_t arrSize>
void sortArray(T (&array)[arrSize]) {
  T hold;
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
