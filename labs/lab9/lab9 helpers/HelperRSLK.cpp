#include "HelperRSLK.h"

SFEVL53L1X distanceSensor;
uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];

/*Returns the encoder pulses to trave x distance.
  Parameters:
  distance (uint32_t): distance to travel.

  Returns:
  int: encoder pulses to drive x distance.
  */
uint32_t countForDistance(float distance) {
  float temp = (WHEEL_DIAMETER * PI) / CNT_PER_REV;
  temp = distance / temp;
  return int(temp);
}

/*Drives the RSLK bot x distance.
  Parameters:
  distance (uint32_t): distance to travel.
  direction (bool): Specifies the direction of the bot.
  wheelSpeedL (uint16_t): The wheel speed at which to turn the motors.

  Returns:
  void
*/
void driveStraight(float distance, bool direction, uint8_t wheelSpeed) {
  if(distance == 0)
    return;

  // Define speed and encoder count variables
  uint8_t wheelSpeedL = wheelSpeed;
  uint8_t wheelSpeedR = wheelSpeed;
  const uint8_t defaultSpeedL = wheelSpeedL;
  const uint8_t defaultSpeedR = wheelSpeedR;

  uint32_t prevLeftCnt = getEncoderLeftCnt();
  uint32_t prevRightCnt = getEncoderRightCnt();

  // Amount of encoder pulses needed to achieve distance
  uint32_t straightLeft = countForDistance(distance) + prevLeftCnt;
  uint32_t straightRight = (countForDistance(distance)-1) + prevRightCnt;

  bool leftStopped = 0;
  bool rightStopped = 0;

  // Set up the motors and encoders
  resetLeftEncoderCnt();  resetRightEncoderCnt();   // Set encoder pulse count back to 0
  setMotorDirection(BOTH_MOTORS,direction); // Cause the robot to drive in the direction specified
  enableMotor(BOTH_MOTORS);
  setRawMotorSpeed(LEFT_MOTOR,wheelSpeedL);
  setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);

  // Drive both motors until both have received the correct number of pulses to travel
  while(!leftStopped || !rightStopped) {

      // if(getEncoderLeftCnt() < (getEncoderRightCnt()-5)) {
      //     wheelSpeedL+=2;
    if(getEncoderLeftCnt() < getEncoderRightCnt()) {
        wheelSpeedL++;
    }


    // if(getEncoderLeftCnt() > getEncoderRightCnt()) {
    //     wheelSpeedL--;
   if(getEncoderLeftCnt() > getEncoderRightCnt()) {
       wheelSpeedL--;
   }

    if (getEncoderLeftCnt() ==  getEncoderRightCnt()) {
     wheelSpeedL = defaultSpeedL;
     wheelSpeedR = defaultSpeedR;
    }

    if (wheelSpeedL > (defaultSpeedL+15))
        wheelSpeedL = defaultSpeedL;

    setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);
    setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);

    if(getEncoderRightCnt() >= (straightRight-prevRightCnt)) {
        disableMotor(RIGHT_MOTOR);
        rightStopped = 1;
    }

    if(getEncoderLeftCnt() >= (straightLeft-prevLeftCnt)) {
        disableMotor(LEFT_MOTOR);
        leftStopped = 1;
    }
    delay(5);
  }

   disableMotor(BOTH_MOTORS);
 }

void turnInPlace(float degrees, bool direction) {
   float distance = (degrees/360)*PI*14;
   uint32_t totalEncoderCount = countForDistance(distance);
   uint8_t wheelSpeedR;
   uint8_t wheelSpeedL;

   // Reset encoder counts
   resetRightEncoderCnt();
   resetLeftEncoderCnt();

   // Turn in place left
   if(direction == LEFT){
     wheelSpeedR = 26;
     wheelSpeedL = 26;
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_BACKWARD);  // Set the left motor to go backwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
  } else {
     wheelSpeedR = 27;
     wheelSpeedL = 27;
     // Turn in place right
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_BACKWARD); // Set the right motor to go backwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);  // Set the left motor to go forwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
  }
  disableMotor(BOTH_MOTORS);
  delay(100);
}

/*Turn in place the smallest amount possible.
  Parameters:
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
void turnInPlaceStatic(uint32_t encoderCount, bool direction) {
   uint32_t totalEncoderCount = encoderCount;

   uint8_t wheelSpeedR;
   uint8_t wheelSpeedL;

   // Reset encoder counts
   resetRightEncoderCnt();
   resetLeftEncoderCnt();

   // Turn in place left
   if(direction == LEFT){
     wheelSpeedR = 28;
     wheelSpeedL = 27;
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_BACKWARD);  // Set the left motor to go backwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
  } else {
     wheelSpeedR = 28;
     wheelSpeedL = 29;
     // Turn in place right
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_BACKWARD); // Set the right motor to go backwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);  // Set the left motor to go forwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
  }
  disableMotor(BOTH_MOTORS);
  delay(50);
}

void tofInit() {
    // Start i2c.
    Wire.begin();

    if (distanceSensor.begin() != 0) {  //Begin returns 0 on a good init.
        Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
        while (1);
  }
  
  Serial.println("Sensor online!");
  distanceSensor.setDistanceModeLong();
  distanceSensor.setTimingBudgetInMs(50); // Set how long is allotted to the measurement.
  distanceSensor.setIntermeasurementPeriod(75); // Set the period between measurements.
}

/* Measures distance in mm using the ToF sensor.
  Returns:
  float: Distance measured in mm.
*/
int measureTOFMM() {
    int distanceArray[15];

    // Get 15 measurements.
    for(uint8_t i=0; i < 15; i++){
        distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
        uint32_t t0 = millis();
        uint32_t t1 = millis();

        // If the data is not ready then keep checking.
        while (!distanceSensor.checkForDataReady()) {
            delay(1);

            // If the time has been greater than 5 seconds than retake measurement.
            if(t1-t0 > 5000) {
                i--;
                continue;
            }
            t1 = millis();
          }


          distanceArray[i] = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

          // If the distance is greater than 350 cm then return 0 as bad value;
          if(distanceArray[i] > 3500) {
              return 0;
          }
          distanceSensor.clearInterrupt();
          distanceSensor.stopRanging();
      }

    // Sort the 15 measurements and take the median.
    sortArray(distanceArray);

    return distanceArray[8];
}

/* Measures distance in cm using the ToF sensor.
  Returns:
  float: Distance measured in cm.
*/
float measureTOFCM() {
    float distanceMM = measureTOFMM();

    return (distanceMM)/10; // Convert mm to cm.
}

 /* Initializes the sensors minimum and maximum values
    Returns:
    void
 */
void initFollowLine() {
    setMotorDirection(BOTH_MOTORS,FORWARD); // Cause the robot to drive in the direction specified
    enableMotor(BOTH_MOTORS);
    resetRightEncoderCnt();
    resetLeftEncoderCnt();

    // Clear the minimum and maxumum.
    clearMinMax(sensorMinVal,sensorMaxVal);

    // Set the minimum and maximum values for the sensors.
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

/* Follows black line and stops on a t.
   Returns:
   void
*/
void followLine() {
    uint16_t normalSpeed =40;
    uint16_t fastSpeed = 60;
    /* Valid values are either:
     *  DARK_LINE  if your floor is lighter than your line
     *  LIGHT_LINE if your floor is darker than your line
     */
    uint8_t lineColor = DARK_LINE;
    /* Run this setup only once */
    readLineSensor(sensorVal);

    setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
    setMotorDirection(LEFT_MOTOR,MOTOR_DIR_BACKWARD);  // Set the left motor to go backwards
    enableMotor(BOTH_MOTORS);                         // "Turn on" the motor
    setRawMotorSpeed(RIGHT_MOTOR, 30);
    setRawMotorSpeed(LEFT_MOTOR, 30);

    // Check the 3rd sensor then the 5th sensor before following line.
    while(sensorVal[3] < 2300) {
        readLineSensor(sensorVal);
    }

    while(sensorVal[5] < 2300) {
        readLineSensor(sensorVal);
    }

    disableMotor(BOTH_MOTORS);
    delay(200);
    readLineSensor(sensorVal);
    enableMotor(BOTH_MOTORS);
    setRawMotorSpeed(RIGHT_MOTOR, 0);
    setRawMotorSpeed(LEFT_MOTOR, 0);
    setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
    setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);

    // follow line until t has been found
    while(1) {
        readLineSensor(sensorVal);

        // When all sensors read black then stop.
        if(sensorVal[0] > 2300 && sensorVal[1] > 2300 && sensorVal[2] > 2300 &&
           sensorVal[3] > 2300 && sensorVal[4] > 2300 && sensorVal[5] > 2300 &&
           sensorVal[6] > 2300 && sensorVal[7] > 2300) {
            disableMotor(BOTH_MOTORS);
            delay(500);
            break;
        }

        readCalLineSensor(sensorVal,sensorCalVal,sensorMinVal,sensorMaxVal,lineColor);
        uint32_t linePos = getLinePosition(sensorCalVal,lineColor);

        if(linePos > 0 && linePos < 3000) {
        setRawMotorSpeed(LEFT_MOTOR,normalSpeed/2);
        setRawMotorSpeed(RIGHT_MOTOR,fastSpeed);
        } else if(linePos > 3500) {
        setRawMotorSpeed(LEFT_MOTOR,fastSpeed);
        setRawMotorSpeed(RIGHT_MOTOR,normalSpeed/2);
        } else {
        setRawMotorSpeed(LEFT_MOTOR,normalSpeed);
        setRawMotorSpeed(RIGHT_MOTOR,normalSpeed);
        }
    }
}
