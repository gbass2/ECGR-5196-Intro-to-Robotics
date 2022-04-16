/*
* Class:       ECGR 4161/5196 Lab 8
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        04-11-2022
* Description: Use servo motor and ultrasonic sensor to identify closest object
*              then drive to it.
*/

#include <Servo.h>
#include "HelperRSLK.h"
Servo myservo;  // create servo object to control a servo

void setup() {
    setupRSLK();  // Set up all of the pins &
                  // functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.begin(38400);
    while(!Serial);

    myservo.attach(38);  // attaches the servo on Port 2.4 to the servo object
    myservo.write(90);    // Send it to the default position
}

void loop() {
    uint8_t wheelSpeed = 60;
    uint16_t degrees = 180; // Degrees to turnInPlace after driving straight.
    uint8_t distanceToWall = 110; // Distance to each wall with error.
    uint16_t distanceToEnd = 300; // cm.
    uint8_t distancePerCheck = 20; // cm
    uint8_t arrSize = distanceToEnd / distancePerCheck);
    arrSize++;
    arrSize-=3;
    float leftArray[arrSize];
    float rightArray[arrSize];
        
    uint8_t counterLeft = 0; // Holds the amount of objects found for the left side.
    uint8_t counterRight = 0; // Holds the amount of objects found for the right side.

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

    // Do an initial check to see if an object is at the starting position.
    // Move servo 90 degrees to starting position.
    for(uint8_t pos = 90; pos > 0; pos -= 5) { // goes from 0 degrees to 90 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 20ms for the servo to reach the position
    }
         
   // Measure distance to the left of the bot.
    leftArray[0] = measureDistance();

    // If an object is found on the left then add to the left counter.
    if(leftArray[0] < distanceToWall)          
      counterLeft++;

    
    delay(500);

    // Move the servo 180 degrees to end position.
    for(uint8_t pos = 0; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 20ms for the servo to reach the position
    }
          
    // Measure distance to the right of the bot.
    rightArray[0] = measureDistance();

    // If an object is found on the right then add to the right counter.
    if(rightArray[0] < distanceToWall)
      counterRight++;
    
    delay(500);
    for(uint8_t pos = 180; pos > 90; pos -= 5) { // goes from 0 degrees to 90 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 20ms for the servo to reach the position
    }
  
  // Loop and drive an increment of 500cm and check to see if object present.
  for(uint8_t i=1; i < arrSize; i++){
    delay(500);
    // Drive the increment
    driveStraight(distancePerCheck, FORWARD,wheelSpeed);
    delay(500);

    // Move servo 90 degrees to starting position.
    for(uint8_t pos = 90; pos > 0; pos -= 5) { // goes from 0 degrees to 90 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 20ms for the servo to reach the position
    }

    delay(500);

    // Measure distance to the left of the bot.
    leftArray[i] = measureDistance();

    // If an object is found on the left then add to the left counter.
    if(leftArray[i] < distanceToWall){
      if(leftArray[i-1] > distanceToWall)
        counterLeft++;
    }

    // Move the servo 180 degrees to end position.
    for(uint8_t pos = 0; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 20ms for the servo to reach the position
    }
    delay(500);
    // Measure distance to the right of the bot.
    rightArray[i] = measureDistance();

    // If an object is found on the right then add to the right counter.
    if(rightArray[i] < distanceToWall){
       if(rightArray[i-1] > distanceToWall)
        counterRight++;
    }
            
    // Move servo 90 degrees to starting position.
    delay(500);
    for(uint8_t pos = 180; pos > 90; pos -= 5) { // goes from 0 degrees to 90 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 20ms for the servo to reach the position
    }
  }

  Serial.println("Left Counter: " + String(counterLeft));            
  Serial.println("Right Counter: " + String(counterRight));     

   for(uint8_t i=0; i<arrSize;i++){
      Serial.println("Left Array: " + String(i) + " " + String(leftArray[i]));            
   }

   for(uint8_t i=0; i<arrSize;i++){
      Serial.println("Right Array: " + String(i) + " " + String(rightArray[i]));            
   }


  // Creating a dynamic array of the objects found.
  float *newLeft = new float(counterLeft);
  float *newRight = new float(counterRight);

  uint8_t leftItr = 0;
  uint8_t rightItr = 0;
  // Loop through original array and find the distances to travel to object.
    for(uint8_t i=0; i < arrSize; i++){
      if(i < distanceToWall) {
        for(uint8_t j=i; j < arrSize; j++){
          if(i > distanceToWall){
            i = j;
            break;
          }
          
        }
      }
    }

  
//  for(uint8_t i=0; i< arrSize; i++){
//    if(leftArray[i] < distanceToWall){
//      if((leftArray[i+1] < distanceToWall && leftArray[i+2] < distanceToWall && leftArray[i+3] < distanceToWall && leftArray[i+4] < distanceToWall) && i < (arrSize-5)){
////        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck +(i+2)*distancePerCheck + (i+3)*distancePerCheck + (i+4)*distancePerCheck)/ 5;
//        float distance = ((i+1)*distancePerCheck +(i+2)*distancePerCheck + (i+3)*distancePerCheck + (i+4)*distancePerCheck)/ 4;
//
//        newLeft[leftItr] = distanceToEnd - (distance);
//        leftItr++;
//
//        if(i < (arrSize-5))
//          i+=5;
//      }
//      else if((leftArray[i+1] < distanceToWall && leftArray[i+2] < distanceToWall && leftArray[i+3] < distanceToWall) && i < (arrSize-4)){
////        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck +(i+2)*distancePerCheck + (i+3)*distancePerCheck)/ 4;
//        float distance = ((i+1)*distancePerCheck +(i+2)*distancePerCheck + (i+3)*distancePerCheck)/ 3;
//
//
//        newLeft[leftItr] = distanceToEnd - (distance);
//        leftItr++;
//
//        if(i < (arrSize-4))
//          i+=4;
//      }
//
//      else if((leftArray[i+1] < distanceToWall && leftArray[i+2] < distanceToWall) && i < (arrSize-3)){
////        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck +(i+2)*distancePerCheck)/ 3;
//        float distance = ((i+1)*distancePerCheck +(i+2)*distancePerCheck)/ 2;
//
//
//        newLeft[leftItr] = distanceToEnd - (distance);
//        leftItr++;
//
//        if(i < (arrSize-3))
//          i+=3;
//      }
//
//      else if((leftArray[i+1] < distanceToWall && leftArray[i+2]) && i < (arrSize-2)){
////        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck)/ 2;
//        float distance = ((i+1)*distancePerCheck);
//
//
//        newLeft[leftItr] = distanceToEnd - (distance);
//        leftItr++;
//        if(i < (arrSize-2))
//          i+=2;
//      }
//      
//      else if(leftArray[i] < distanceToWall){
//        newLeft[leftItr] = distanceToEnd - ((i)*distancePerCheck);
//        leftItr++;
//      }
//    }
//    
//    if(rightArray[i] < distanceToWall){
//      if((rightArray[i+1] < distanceToWall && rightArray[i+2] < distanceToWall && rightArray[i+3] < distanceToWall && rightArray[i+4] < distanceToWall) && i < (arrSize-5)){
//        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck +(i+2)*distancePerCheck + (i+3)*distancePerCheck + (i+4)*distancePerCheck)/ 5;
//        newRight[rightItr] = distanceToEnd - (distance);
//        rightItr++;
//
//        if(i < (arrSize-5))
//          i+=5;
//      }
//      else if((rightArray[i+1] < distanceToWall && rightArray[i+2] < distanceToWall && rightArray[i+3] < distanceToWall) && i < (arrSize-4)){
//        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck +(i+2)*distancePerCheck + (i+3)*distancePerCheck)/ 4;
//
//        newRight[rightItr] = distanceToEnd - (distance);
//        rightItr++;
//
//        if(i < (arrSize-4))
//          i+=4;
//      }
//
//      else if((rightArray[i+1] < distanceToWall && rightArray[i+2] < distanceToWall) && i < (arrSize-3)){
//        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck +(i+2)*distancePerCheck)/ 3;
//
//        newRight[rightItr] = distanceToEnd - (distance);
//        rightItr++;
//
//        if(i < (arrSize-3))
//          i+=3;
//      }
//
//      else if((rightArray[i+1] < distanceToWall && rightArray[i+2]) && i < (arrSize-2)){
//        float distance = ((i)*distancePerCheck + (i+1)*distancePerCheck)/ 2;
//
//        newRight[rightItr] = distanceToEnd - (distance);
//        rightItr++;
//        if(i < (arrSize-2))
//          i+=2;
//      }
//      
//      else if(rightArray[i] < distanceToWall){
//        newRight[rightItr] = distanceToEnd - ((i)*distancePerCheck);
//        rightItr++;
//      }
//    }
//    
//  }

  for(uint8_t i=0; i < counterLeft; i++) {
    Serial.println("Left distances: " + String(newLeft[i]));
  }

  for(uint8_t i=0; i < counterRight; i++) {
    Serial.println("Right distances: " + String(newRight[i]));
  }

  delay(3000);
  
  // Turn 180 degrees to the right.
  turnInPlace(degrees, RIGHT);

  

  leftItr = counterLeft;
  rightItr = counterRight;
  uint16_t distanceDriven = 0;

  while(leftItr > 0 && rightItr > 0){
    if (newLeft[leftItr] < newRight[rightItr]) {
      driveStraight(newLeft[leftItr]-distanceDriven, FORWARD,wheelSpeed);
      distanceDriven = newLeft[rightItr];
      leftItr--;
      Serial.println("distance to travel Left: " + String(newLeft[leftItr]-distanceDriven));
      Serial.println("distanceDriven: " + String(distanceDriven));
      delay(3000);
    }
    else if (newRight[rightItr] < newLeft[leftItr]) {
      driveStraight(newRight[rightItr]-distanceDriven, FORWARD,wheelSpeed);
      distanceDriven = newRight[rightItr];
      rightItr--;
      Serial.println("distanceDriven Right: " + String(distanceDriven));
      Serial.println("distance to travel: " + String(newRight[leftItr]-distanceDriven));

      delay(3000);
    }
  }
}
