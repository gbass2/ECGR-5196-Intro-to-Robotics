#include "HelperRSLK.h"

void setup()
{
  setupRSLK();
  Serial.begin(38400);
  while(!Serial);

  initFollowLine();
  
  /* Left button on Launchpad */
  setupWaitBtn(LP_LEFT_BTN);
  /* Red led in rgb led */
  setupLed(RED_LED);
  waitBtnPressed(LP_LEFT_BTN,"",RED_LED);
}

void loop() {
    followLine();
}
