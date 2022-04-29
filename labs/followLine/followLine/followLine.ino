#include "HelperRSLK.h"

uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];
void setup()
{
  setupRSLK();
  Serial.begin(38400);
  while(!Serial);
  /* Left button on Launchpad */
  setupWaitBtn(LP_LEFT_BTN);
  /* Red led in rgb led */
  setupLed(RED_LED);
  clearMinMax(sensorMinVal,sensorMaxVal);
}
void floorCalibration() {
  /* Place Robot On Floor (no line) */
  delay(2000);
  String btnMsg = "Push left button on Launchpad to begin calibration.\n";
  btnMsg += "Make sure the robot is on the floor away from the line.\n";  /* Wait until button is pressed to start robot */
  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  delay(1000);
  Serial.println("Running calibration on floor");
  simpleCalibrate();
  Serial.println("Reading floor values complete");
  btnMsg = "Push left button on Launchpad to begin line following.\n";
  btnMsg += "Make sure the robot is on the line.\n";
  /* Wait until button is pressed to start robot */
//  waitBtnPressed(LP_LEFT_BTN,btnMsg,RED_LED);
  delay(1000);
  enableMotor(BOTH_MOTORS);
}
void simpleCalibrate() {
  /* Set both motors direction forward */
  setMotorDirection(BOTH_MOTORS,MOTOR_DIR_FORWARD);
  /* Enable both motors */
//  enableMotor(BOTH_MOTORS);
  /* Set both motors speed 20 */
//  setMotorSpeed(BOTH_MOTORS,20);
  for(int x = 0;x<100;x++){
  readLineSensor(sensorVal);
//  setSensorMinMax(sensorVal,sensorMinVal,sensorMaxVal);
  sensorMinVal[0] = 993;
  sensorMinVal[1] = 1019;
  sensorMinVal[2] = 1002;
  sensorMinVal[3] = 1003;
  sensorMinVal[4] = 874;
  sensorMinVal[5] = 1478;
  sensorMinVal[6] = 840;
  sensorMinVal[7] = 1065;

  sensorMaxVal[0] = 1174;
  sensorMaxVal[1] = 1264;
  sensorMaxVal[2] = 1350;
  sensorMaxVal[3] = 2500;
  sensorMaxVal[4] = 2500;
  sensorMaxVal[5] = 1829;
  sensorMaxVal[6] = 1010;
  sensorMaxVal[7] = 1305;
  }

  for(uint8_t i=0; i < 8; i++){
    Serial.println("Min val: " + String(sensorMinVal[i]));

  }

   for(uint8_t i=0; i < 8; i++){
    Serial.println("Max val: " + String(sensorMaxVal[i]));

  }
  /* Disable both motors */
//  disableMotor(BOTH_MOTORS);
}

bool isCalibrationComplete = false;

void loop() {
    uint16_t normalSpeed = 8;
    uint16_t fastSpeed = 20;
    /* Valid values are either:
     *  DARK_LINE  if your floor is lighter than your line
     *  LIGHT_LINE if your floor is darker than your line
     */
    uint8_t lineColor = DARK_LINE;
    /* Run this setup only once */
    if(isCalibrationComplete == false) {
    floorCalibration();
    isCalibrationComplete = true;
    }
    readLineSensor(sensorVal);
    if(sensorVal[0] > 2300 && sensorVal[1] > 2300 && sensorVal[2] > 2300 &&
       sensorVal[3] > 2300 && sensorVal[4] > 2300 && sensorVal[5] > 2300 &&
       sensorVal[6] > 2300 && sensorVal[7] > 2300) {
        disableMotor(BOTH_MOTORS);
        delay(500);
       }
        
    readCalLineSensor(sensorVal,sensorCalVal,sensorMinVal,sensorMaxVal,lineColor);
    uint32_t linePos = getLinePosition(sensorCalVal,lineColor);
//    Serial.println(linePos);
    if(linePos > 0 && linePos < 3000) {
    setMotorSpeed(LEFT_MOTOR,normalSpeed);
    setMotorSpeed(RIGHT_MOTOR,fastSpeed);
    } else if(linePos > 3500) {
    setMotorSpeed(LEFT_MOTOR,fastSpeed);
    setMotorSpeed(RIGHT_MOTOR,normalSpeed);
    } else {
    setMotorSpeed(LEFT_MOTOR,normalSpeed);
    setMotorSpeed(RIGHT_MOTOR,normalSpeed);
    }
}
