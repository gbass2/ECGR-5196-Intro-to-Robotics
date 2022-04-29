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

    // Move the servo 180 degrees to end position.
    myservo.write(180);

    delay(750);

    // Move servo 90 degrees to starting position.
      myservo.write(90);

    delay(500);

  }
}
