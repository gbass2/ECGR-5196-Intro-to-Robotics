/*
* Class:       ECGR 4161/5196 Lab 8
* Authors:     Grayson Bass, Ryan Roscrow
* Date:        04-18-2022
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
    uint8_t distanceToWall = 110; //cm. Distance to each wall with error.
    uint16_t distanceToEnd = 220; // cm.
    uint8_t distancePerCheck = 20; // cm. Holds the distance to drive between pings.
    uint8_t arrSize = distanceToEnd / distancePerCheck;
    float leftArray[arrSize]; // Holds the pings on the left side of the bot.
    float rightArray[arrSize]; // Holds the pings on the right side of the bot.
        
    uint8_t counterLeft = 0; // Holds the amount of objects found for the left side.
    uint8_t counterRight = 0; // Holds the amount of objects found for the right side.

    // Waiting for button to be pressed.
    waitBtnPressed(LP_LEFT_BTN,"",BLUE_LED);
    delay(2000);

  // Loop and drive an increment of 300cm and check to see if object present.
  for(uint8_t i=0; i < arrSize; i++){
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

    delay(500);
    // Drive the increment
    driveStraight(distancePerCheck, FORWARD,wheelSpeed);
    delay(500);
  }

  Serial.println("Left Counter: " + String(counterLeft));            
  Serial.println("Right Counter: " + String(counterRight));     

   for(uint8_t i=0; i<arrSize;i++){
      Serial.println("Left Array: " + String(i) + " " + String(leftArray[i]));            
   }

   for(uint8_t i=0; i<arrSize;i++){
      Serial.println("Right Array: " + String(i) + " " + String(rightArray[i]));            
   }

  // Creating a dynamicly allocated array of number of objects found.
  float *newLeft = new float(counterLeft);
  float *newRight = new float(counterRight);

  uint8_t leftItr = 0;
  uint8_t rightItr = 0;
  float distToObj = 0; // Distance to current detected object.
  
  // Loop through original array and find the distances to travel to the objects.
    for(uint8_t i=0; i < arrSize; i++){
      // Take the average of the distances to travel to the object if multiple pings find same object.
      // For left array.
      if(leftArray[i] < distanceToWall) {
        for(uint8_t j=i; j < arrSize; j++){
          if(leftArray[j] > distanceToWall){
            newLeft[leftItr] = distanceToEnd - (distToObj/(j-i)); // Take the average of the distances.

            if(leftItr < (counterLeft-1))
              leftItr++;
              
            i = j;
            break;
          }
         
          distToObj+=j*distancePerCheck;

          if(j==(arrSize-1)){
            newLeft[leftItr] = distanceToEnd - (distToObj/((j-i)+1)); // Take the average of the distances.
            i=j;
            break;
          }
        }
        distToObj = 0;
      }

       // For right array.
       if(rightArray[i] < distanceToWall) {
        for(uint8_t j=i; j < arrSize; j++){
          if(rightArray[j] > distanceToWall){
            newRight[rightItr] = distanceToEnd - (distToObj/(j-i)); // Take the average of the distances.

            if(rightItr < (counterRight-1))
              rightItr++;
              
            i = j;
            break;
          }
          distToObj+=j*distancePerCheck;

          if(j==(arrSize-1)){
            newRight[rightItr] = distanceToEnd - (distToObj/((j-i)+1)); // Take the average of the distances.
            i=j;
            break;
          }
        }
        distToObj = 0;
      }
    }

  for(uint8_t i=0; i < counterLeft; i++) {
    Serial.println("Left distances: " + String(newLeft[i]));
  }

  for(uint8_t i=0; i < counterRight; i++) {
    Serial.println("Right distances: " + String(newRight[i]));
  }

  delay(3000);

  // Turn 180 degrees to the right.
  turnInPlace(degrees, RIGHT);
  
  uint16_t distanceDriven = 0; // Holds the distance the bot has driven for the trip back.
  
  // Drive to each object and turn servo toward object.
  while(leftItr >= 0 && rightItr >= 0){
    if (newLeft[leftItr] < newRight[rightItr] && leftItr >= 0) {
      uint16_t distToDrive = newLeft[leftItr]-distanceDriven;
      driveStraight(distToDrive,FORWARD,wheelSpeed);
      distanceDriven = newLeft[leftItr];

      if(leftItr != 0)
        leftItr--;
      else{
        newLeft[leftItr] = 999;
      }
      
      Serial.println("LeftItr: " + String(leftItr));
      Serial.println("distance to travel Left: " + String(distToDrive));
      Serial.println("distanceDriven: " + String(distanceDriven));
  
      // Move servo to left object
      for(uint8_t pos = 90; pos < 180; pos += 5) { // goes from 0 degrees to 180 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 20ms for the servo to reach the position
      }

      delay(2000);
      
      // Move servo to to start.
      for(uint8_t pos = 180; pos > 90; pos -= 5) { // goes from 0 degrees to 90 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 20ms for the servo to reach the position
      }
    } 
    
    else if (newRight[rightItr] < newLeft[leftItr] && rightItr >= 0) {
      uint16_t distToDrive = newRight[rightItr]-distanceDriven;
      driveStraight(distToDrive, FORWARD,wheelSpeed);
      distanceDriven = newRight[rightItr];

      if(rightItr !=0)
        rightItr--;
      else{
        newRight[rightItr] = 999;
      }

      Serial.println("RightItr: " + String(rightItr));
      Serial.println("distance to travel Right: " + String(distToDrive));
      Serial.println("distanceDriven Right: " + String(distanceDriven));

      // Move servo to right object.
      for(uint8_t pos = 90; pos > 0; pos -= 5) { // goes from 0 degrees to 90 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 20ms for the servo to reach the position
     }

      delay(2000);

      // Move servo to start.
      for(uint8_t pos = 0; pos < 90; pos += 5) { // goes from 0 degrees to 180 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 20ms for the servo to reach the position
      }
    }

    else {
      uint16_t distToDrive = distanceToEnd - distanceDriven;
      driveStraight(distToDrive, FORWARD,wheelSpeed);
      break;
    }
  }
}
