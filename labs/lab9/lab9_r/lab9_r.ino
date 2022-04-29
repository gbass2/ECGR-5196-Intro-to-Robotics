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


void setup() {
    setupRSLK();  // Set up all of the pins &
                  // functions needed to be used by the TI bot.
    setupWaitBtn(LP_LEFT_BTN); // Set up left button.
    setupLed(BLUE_LED); // Setup the blue led.

    tofInit();

    Serial.begin(38400);
    while(!Serial);
    
    myservo.attach(38);  // attaches the servo on Port 2.4 to the servo object
    myservo.write(84);    // Send it to the default position
    delay(2000);

}

void loop() {

    // Waiting for button to be pressed.
    int distanceToCenter[4];
    distanceToCenter[0] = measureTOFMM();
    
    delay(250);
    int degrees = 180;
    turnInPlace(degrees, RIGHT);
//    
    myservo.write(0);
    delay(500);
    distanceToCenter[1] = measureTOFMM();
    delay(250);
//
    myservo.write(84);
    delay(500);
    distanceToCenter[2] = measureTOFMM();
    delay(250);
    
    myservo.write(180);
    delay(500);
    distanceToCenter[3] = measureTOFMM();
    delay(250);
//
    myservo.write(84);
    delay(500);
//
    degrees = 90;
//
    for(auto i: distanceToCenter) {
          Serial.println(i);
    }

    delay(5000);
}
