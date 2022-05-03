/*
* Class:       ECGR 4161/5196 Lab 9
* Authors:     Grayson Bass
* Date:        05-02-2022
* Description: Localizes bot inside a room. 
*              Finds the center of the room.
*              Finds a line and follows to a maze.
*              Traverses maze.
*/

#include <Servo.h>
#include "SimpleRSLK.h"
#include "HelperRSLK.h"

Servo myservo;  // create servo object to control a servo

/* Finds the perpendicular wall to the bot.
  Returns:
  void
*/
void findPerpendicular() {
    int distToWall[2]; // Array to store the unsorted distances.
    bool isPerp = false; // Set to true when the perpendicular distance is found.
    float encoderCount = 40;

    while(1) {
      // If the perpendicular distance is found then break.
      if(abs(distToWall[0] - distToWall[1]) < 10 && distToWall[0] > 0 && distToWall[1]  > 0)
        break;

      // Turn left and measure.
      turnInPlaceStatic(encoderCount,LEFT);
      distToWall[0] = measureTOFMM();
      delay(100);

      // Turn right num of encoderTicks to the right of initial point.
      turnInPlaceStatic(2*encoderCount,RIGHT);
      distToWall[1] = measureTOFMM();
      delay(100);

      // Turn back to initial point.
      turnInPlaceStatic(encoderCount,LEFT);
      delay(100);
  
      Serial.println(String(distToWall[0]) + '\t' + distToWall[1]);

      // If perpendicular is found set flag to true.
      if(abs((distToWall[0] - distToWall[1])) < 10){
        isPerp = true;
      }
      
      // If the left distance is less than right distance turn to the left towards perpendicular.
      if(distToWall[0] < distToWall[1]){ 
        if(!isPerp) {
          Serial.println((abs(distToWall[0] - distToWall[1]) > 50));
          if(abs(distToWall[0] - distToWall[1]) > 50) {
            turnInPlaceStatic(encoderCount/2,LEFT);
          } else {
             turnInPlaceStatic(3,LEFT);
          }
        }

      // If the left distance is greater than right distance turn to the right towards perpendicular.
      } else {
        if(!isPerp) {
          if(abs(distToWall[0] - distToWall[1]) > 50) {
            turnInPlaceStatic(encoderCount/2,RIGHT);
          } else {
            turnInPlaceStatic(3,RIGHT);
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

    
    // Find the 4 measurements to localize. forwards towards perpendicular wall, 180 of the perpendicular,
    // and left and right after turning 180.
    float distanceToCenter[4];
    distanceToCenter[0] = measureTOFCM();
    delay(500);
    degrees = 177;
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

    degrees = 90;
    
    // Drive to the middle of both distances.
    float distanceToMiddle = 0;

    // drive to the center of the room perpendicular to the shortest wall found.
    distanceToMiddle = ((distanceToCenter[0] + distanceToCenter[2] + BOT_LENGTH))/2;
    distanceToMiddle-=distanceToCenter[0];
    distanceToMiddle-=7;

    driveStraight(distanceToMiddle, FORWARD, wheelSpeed);
    delay(100);

    // Turn to the right if the left distance is less than right and the distance towards 
    // perpendicular wall is less than the opposite wall.
    if(distanceToCenter[1] < distanceToCenter[3] && distanceToCenter[0] < distanceToCenter[2]) {
      turnInPlace(degrees, RIGHT);
    } else {
      turnInPlace(degrees+3, LEFT);
    }

    // Calculate the distance to drive between the left and right walls of the perpendicular.
    if(distanceToCenter[1] < distanceToCenter[3]) {
      distanceToMiddle = ((distanceToCenter[1] + distanceToCenter[3] - FULL_BOT_WIDTH))/2;
      distanceToMiddle-=distanceToCenter[1];
      distanceToMiddle+=3;
      // Turn Left.
    } else {
      distanceToMiddle = ((distanceToCenter[1] + distanceToCenter[3] - FULL_BOT_WIDTH))/2;
      distanceToMiddle-=distanceToCenter[3];
      distanceToMiddle+=3;
    }

    driveStraight(distanceToMiddle, FORWARD, wheelSpeed);
    delay(100);
}

bool initCheck  = true; // Flag for when the maze is in its first iteration.
                        // Only worked when declared as global.

/* Traverses the maze.
  Returns:
  void
*/
void traverseMaze() {
  uint8_t wheelSpeed = 40;
  int holeThreshold = 200; // The distance to detect an exit point.
  int forwardThreshold = 14; // The amound of distance used  to determine when to turn right straight ahead.
  int leftThreshold = 40; // The amound of distance used to determine when to turn right to the left of the bot.
  float distances[3]; // Holds the three measurements.
  float distanceToDrive; // Distance to drive after each set of measurments.
  uint8_t degrees = 90;
  bool holeFound = false;
  bool direction = LEFT;

  // Loop indefinately. 
  while(!holeFound) {
      // Get straight measurement.
      myservo.write(84);
      distances[0] = measureTOFCM();
      delay(25);
  
      // Turn servo left and get measurment.
      myservo.write(0);
      distances[1] = measureTOFCM();
      delay(25);

      // Turn servo right and get measurement.
      myservo.write(180);
      distances[2] = measureTOFCM();
      delay(100);
      direction = RIGHT;

    myservo.write(84);
    
    // If the left distance greater than straight and right then turn left and drive.
    if(distances[1] > distances[0] && distances[1] > distances[2]) {
      distanceToDrive = distances[1];

      if(!initCheck) {
        if(distances[1] < holeThreshold)
          distanceToDrive=20;
        else {
          distanceToDrive=10;
        }
        driveStraight(distanceToDrive, FORWARD, wheelSpeed);
      }

      distanceToDrive=20;
      
      turnInPlace(degrees, LEFT);
      delay(100);
      driveStraight(distanceToDrive, FORWARD, wheelSpeed);

    // If the right is greater than the hole threshold then found the exit. Turn right and drive distance.
    } else if(distances[2] >= holeThreshold) {
      distanceToDrive = 3;
      driveStraight(distanceToDrive, FORWARD, wheelSpeed);
      delay(100);
      turnInPlace(degrees, RIGHT);
      delay(100);
      distanceToDrive = distances[2];
      driveStraight(distanceToDrive, FORWARD, wheelSpeed);

      // If straight and left are less than their thresholds then robot is in corner. Turn right.
    } else if(distances[0] <= forwardThreshold && distances[1] <= leftThreshold) {
      turnInPlace(degrees, RIGHT);
      
    // If cannot do either than go straight.
    } else {
      if(distances[1] > 30)
        turnInPlaceStatic(10, LEFT);
      else if(distances[1] < 20) {
        turnInPlaceStatic(10, RIGHT);
      }
      
      distanceToDrive=5;
      
      driveStraight(distanceToDrive, FORWARD, wheelSpeed);
    }
    
    initCheck=false;
    }
}


void setup() {
    setupRSLK();  // Set up all of the pins &
                  // functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.

    // Initialize the line following and tof.
    initFollowLine();
    tofInit();


    Serial.begin(38400);
    while(!Serial);
    delay(200);

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

    // Traverse the maze.
    traverseMaze();
}
