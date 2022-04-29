/*
* Class:       ECGR 4161/5196 Grad Lab
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        04-18-2022
* Description: Use servo motor and time of flight sensor to identify objects
*              Allong the left and right side of the robot driving 300cm.
*/
#include <Servo.h>
#include "HelperRSLK.h"
Servo myservo;  // create servo object to control a servo


void setup() {
    
    Serial.begin(38400);
    while(!Serial);

    setupRSLK();  // Set up all of the pins
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.
    
    myservo.attach(38);  // attaches the servo on Port 2.4 to the servo object
    myservo.write(90);    // Send it to the default position

    resetLeftEncoderCnt();  resetRightEncoderCnt();   // Set encoder pulse count back to 0

    // Initialize the ToF sensor
    tofInit();
 }

void loop() {
    uint8_t  wheelSpeed = 30;
    uint16_t degrees = 180; // Degrees to turnInPlace after driving straight.
    uint16_t distanceToWall = 110; //cm. Distance to each wall with error.
    uint16_t distanceToEnd = 300; // cm. Total distance to drive.
    uint8_t  distancePerCheck = 20; // cm. Holds the distance to drive between pings.
    uint8_t  arrSize = distanceToEnd / distancePerCheck;
    float    leftArray[arrSize]; // Holds the pings on the left side of the bot.
    float    rightArray[arrSize]; // Holds the pings on the right side of the bot.

    uint8_t  counterLeft = 0; // Holds the amount of objects found for the left side.
    uint8_t  counterRight = 0; // Holds the amount of objects found for the right side.

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(1000);

  // Loop and drive an increment of 300cm and check to see if object present.
  for(uint8_t i=0; i < arrSize; i++){
    // Move servo 90 degrees to starting position.
    myservo.write(0);
    delay(500);

    // Measure distance to the left of the bot.
    leftArray[i] = measureTOFCM();

    // If an object is found on the left then add to the left counter.
    if(leftArray[i] < distanceToWall){
      if(leftArray[i-1] > distanceToWall)
        counterLeft++;
    }

    // Move the servo 180 degrees to end position.
    myservo.write(180);
    delay(750);

    // Measure distance to the right of the bot.
    rightArray[i] = measureTOFCM();

    // If an object is found on the right then add to the right counter.
    if(rightArray[i] < distanceToWall){
       if(rightArray[i-1] > distanceToWall)
        counterRight++;
    }

    // Move servo 90 degrees to starting position.
    myservo.write(90);
    delay(500);

    // Drive the increment
    driveStraight(distancePerCheck, FORWARD,wheelSpeed);
    delay(500);
  }

  for(uint8_t i=0; i<arrSize; i++) {
    Serial.println("Left Array: " + String(leftArray[i]) + '\t');
  }

  for(uint8_t i=0; i<arrSize; i++) {
    Serial.println("Right Array: " + String(rightArray[i]) + '\t');
  }

  // Creating a dynamicly allocated array of number of objects found.
  float *newLeft = new float(counterLeft);
  float *newRight = new float(counterRight);

  uint8_t leftItr = 0;
  uint8_t rightItr = 0;
  float distToObj = 0; // Distance to current detected object.

  // Loop through original array and find the distances to travel to the objects.
  // For left array.
    for(uint8_t i=0; i < arrSize; i++){
      // Take the average of the distances to travel to the object if multiple pings find same object.
      if(leftArray[i] < distanceToWall) {
        for(uint8_t j=i; j < arrSize; j++){
          if(leftArray[j] > distanceToWall){
            newLeft[leftItr] = distanceToEnd - (distToObj/(j-i)); // Take the average of the distances.

            if(leftItr < (counterLeft-1))
              leftItr++;

            i = j;
            break;
          }

          distToObj+=j*distancePerCheck; // Sum up all the distances to travel.

          // Handles edge case where an object is at the end of the 300cm drive.
          if(j==(arrSize-1)){
            newLeft[leftItr] = distanceToEnd - (distToObj/((j-i)+1)); // Take the average of the distances.
            i=j;
            break;
          }
        }
        distToObj = 0;
      }
    }

    // For right array.
    for(uint8_t i=0; i < arrSize; i++){
       if(rightArray[i] < distanceToWall) {
        for(uint8_t j=i; j < arrSize; j++){
          if(rightArray[j] > distanceToWall){
            newRight[rightItr] = distanceToEnd - (distToObj/(j-i)); // Take the average of the distances.

            if(rightItr < (counterRight-1))
              rightItr++;

            i = j;
            break;
          }
          distToObj+=j*distancePerCheck; // Sum up all the distances to travel.

          // Handles edge case where an object is at the end of the 300cm drive.
          if(j==(arrSize-1)){
            newRight[rightItr] = distanceToEnd - (distToObj/((j-i)+1)); // Take the average of the distances.
            i=j;
            break;
          }
        }
        distToObj = 0;
      }
    }

  // Turn 180 degrees to the right.
  turnInPlace(degrees, RIGHT);

  uint16_t distanceDriven = 0; // Holds the distance the bot has driven for the trip back.

  // Drive to each object and turn servo toward object.
  // ****Add a case where newLeft[leftItr] == newRight[rightItr]****
  while(leftItr >= 0 && rightItr >= 0){
    // Drive to obeject on left if it is closer than the right.
    if (newLeft[leftItr] < newRight[rightItr] && leftItr >= 0) {
      uint16_t distToDrive = newLeft[leftItr]-distanceDriven;
      driveStraight(distToDrive,FORWARD,wheelSpeed);
      distanceDriven = newLeft[leftItr];

      if(leftItr != 0)
        leftItr--;
      else{
        newLeft[leftItr] = 999;
      }

      // Move servo to left object
      myservo.write(180);              // tell servo to go to position 180 deg.

      delay(2000);

      // Move servo to to start.
      myservo.write(90);              // tell servo to go to position 90 deg.
    }

    // Drive to obeject on right if it is closer than the left.
    else if (newRight[rightItr] < newLeft[leftItr] && rightItr >= 0) {
      uint16_t distToDrive = newRight[rightItr]-distanceDriven;
      driveStraight(distToDrive, FORWARD,wheelSpeed);
      distanceDriven = newRight[rightItr];

      if(rightItr !=0)
        rightItr--;
      else{
        newRight[rightItr] = 999;
      }

      // Move servo to right object.
      myservo.write(0);              // tell servo to go to position 0.

      delay(2000);

      // Move servo to start.
      myservo.write(90);              // tell servo to go to position 90.

    }

    // If all obejcects found, drive to start.
    else {
      uint16_t distToDrive = distanceToEnd - distanceDriven;
      driveStraight(distToDrive, FORWARD,wheelSpeed);
      break;
    }
  }
}
