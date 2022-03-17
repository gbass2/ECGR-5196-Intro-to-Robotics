/*
* Class:       ECGR 4161/5196 ultrasonic example
* Authors:      Grayson Bass, Ryan Roscrow
* Date:        02-28-2022
* Description: Ultrasonic example
*/

#include "SimpleRSLK.h"
#include "HelperRSLK.h"

void setup() {
 setupRSLK();  // Set up all of the pins & functions needed to be used by the TI bot
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 Serial.begin(38400);
 while(!Serial);
}

void loop() {
  Serial.println(measureDistance());
  delay(500);
}
