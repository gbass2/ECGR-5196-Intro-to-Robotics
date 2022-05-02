#include <Servo.h>
#include "HelperRSLK.h"

void setup() {
    
    Serial.begin(38400);
    while(!Serial);
    setupRSLK();  // Set up all of the pins
    delay(2000);
    
    // Initialize the ToF sensor
    tofInit();
 }

void loop() {
  Serial.println(measureTOFCM());
//  measureTOFCM();
  delay(500);
}
