/*
* Class:       ECGR 4161/5196 Lab 6
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        02-28-2022
* Description:
*/

#include <Servo.h>
#include "SimpleRSLK.h"
#include "HelperRSLK.h"

Servo myservo;  // create servo object to control a servo

// Functions that use the servo.

/* Finds the perpendicular wall to the bot.
  Returns:
  void
*/
void findPerpendicular() {
    float degrees = 90; // Degrees to turnInPlace after driving straight.
    int distToWall[2]; // Array to store the unsorted distances.
    bool isPerp = false;
    float encoderCount = 40;

    while(1) {
    if(abs(distToWall[0] - distToWall[1]) < 10 && distToWall[0] > 0 && distToWall[1]  > 0)
      break;

    turnInPlaceStatic(encoderCount,LEFT);
    distToWall[0] = measureTOFMM();
    delay(150);
    
    turnInPlaceStatic(2*encoderCount,RIGHT);
    distToWall[1] = measureTOFMM();
    delay(150);

    turnInPlaceStatic(encoderCount,LEFT);
    delay(150);

    Serial.println(String(distToWall[0]) + '\t' + distToWall[1]);

    if(abs((distToWall[0] - distToWall[1])) < 10){
      isPerp = true;
    }

    if(distToWall[0] < distToWall[1]){ 
      if(!isPerp) {
        Serial.println((abs(distToWall[0] - distToWall[1]) > 50));
        if(abs(distToWall[0] - distToWall[1]) > 50) {
          turnInPlaceStatic(encoderCount/2,LEFT);
        } else {
           turnInPlaceStatic(encoderCount/5,LEFT);
        }
      }
    } else {
      if(!isPerp) {
        if(abs(distToWall[0] - distToWall[1]) > 50) {
          turnInPlaceStatic(encoderCount/2,RIGHT);
        } else {
          turnInPlaceStatic(encoderCount/5,RIGHT);
        }
      }
    }
  }
}


/* Finds the center of the room and drives to it.
  Returns:
  void
*/
void localizeRoom() {
    uint8_t wheelSpeed = 30;
    float degrees = 90; // Degrees to turnInPlace after driving straight.
    float botLength = 5.25; // Length of the bot.
    float botWidth = 0; // Length of the bot.

    float distanceToCenter[4];
    distanceToCenter[0] = measureTOFCM();
    delay(500);
    degrees = 180;
    turnInPlace(degrees, RIGHT);
    myservo.write(0);
    distanceToCenter[1] = measureTOFCM();
    delay(750);


    myservo.write(84);
    distanceToCenter[2] = measureTOFCM();
    delay(750);
    
    myservo.write(180);
    distanceToCenter[3] = measureTOFCM();
    delay(750);

    myservo.write(84);

    degrees = 90;

    for(auto i: distanceToCenter) {
          Serial.println(i);
    }

    
    // Drive to the middle of both dstances.
    uint16_t distanceToMiddle;
    if(distanceToCenter[0] < distanceToCenter[2]) {
      distanceToMiddle = ((distanceToCenter[0] + distanceToCenter[2] + botLength))/2;
      distanceToMiddle-=distanceToCenter[0];
    } else {
      distanceToMiddle = ((distanceToCenter[0] + distanceToCenter[2] + botLength))/2;
      distanceToMiddle-=distanceToCenter[2];
    }

    driveStraight(distanceToMiddle, FORWARD, wheelSpeed);
    delay(100);

    if(distanceToCenter[1] < distanceToCenter[3] && distanceToCenter[0] < distanceToCenter[2]) {
      turnInPlace(degrees, RIGHT);
    } else {
      turnInPlace(degrees, LEFT);
    }

    if(distanceToCenter[1] < distanceToCenter[3]) {
      distanceToMiddle = ((distanceToCenter[1] + distanceToCenter[3] + botWidth))/2;
      distanceToMiddle-=distanceToCenter[1];
    } else {
      distanceToMiddle = ((distanceToCenter[1] + distanceToCenter[3] + botWidth)-distanceToCenter[3])/2;
      distanceToMiddle-=distanceToCenter[3];
    }

    driveStraight(distanceToMiddle, FORWARD, wheelSpeed);
    delay(100);
}


void setup() {
    setupRSLK();  // Set up all of the pins &
                  // functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.

    initFollowLine();
    tofInit();

    Serial.begin(38400);
    while(!Serial);
    delay(2000);

    myservo.attach(38);  // attaches the servo on Port 2.4 to the servo object
    myservo.write(84);    // Send it to the default position

   // Reset encoder counts
   resetRightEncoderCnt();
   resetLeftEncoderCnt();
}

void loop() {
    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

    // Find the perpendicular wall.
    findPerpendicular();

    // Localize and find center.
    localizeRoom();

    // Follow the line to the maze.
    followLine();
}
