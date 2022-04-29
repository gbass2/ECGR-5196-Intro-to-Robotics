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
  uint8_t wheelSpeedL = wheelSpeed+1;
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

    if(getEncoderLeftCnt() < getEncoderRightCnt()) {
        wheelSpeedL++;
    }

   if(getEncoderLeftCnt() > getEncoderRightCnt()) {
       wheelSpeedL--;
    }

    if (getEncoderLeftCnt() ==  getEncoderRightCnt()) {
     wheelSpeedL = defaultSpeedL;
     wheelSpeedR = defaultSpeedR;
    }

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
   delay(500);
 }


/*Pivots the x degrees.
  Parameters:
  degrees (uint16_t): degrees to pivot.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
// void pivot(uint16_t degrees, bool direction) {
//   // Calculating the distance to pivot based on the degrees defined.
//   uint16_t distance = round((float(degrees)/360)*PI*28);
//   uint32_t totalEncoderCount = countForDistance(distance);
//   uint8_t wheelSpeed = 10; // Setting wheel speed
//
//   // Reset the encoder counts
//   resetRightEncoderCnt();                           // Set encoder pulse count back to 0
//   resetLeftEncoderCnt();
//
//   // Pivoting left
//   if(direction == LEFT){
//    // Set up the motors and encoders
//    setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Cause the robot to drive forward
//    enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
//    setMotorSpeed(RIGHT_MOTOR, wheelSpeed);
//
//    // Drive R motor until it has received the correct number of pulses to travel
//    while((getEncoderRightCnt()<totalEncoderCount));       // stay in loop
//    disableMotor(RIGHT_MOTOR);
//   } else {
//    // Pivot right
//    // Set up the motors and encoders
//    setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD); // Cause the robot to drive forward
//    enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
//    setMotorSpeed(LEFT_MOTOR, wheelSpeed);
//    delay(500);
//
//    // Drive L motor until it has received the correct number of pulses to travel
//    while(getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
//    disableMotor(LEFT_MOTOR);
//    delay(500);
//   }
// }


/*Turn in place the x degrees.
  Parameters:
  degrees (uint16_t): degrees to pivot.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
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
     wheelSpeedR = 27;
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
  delay(500);
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
  delay(500);
}

/*Drives the RSLK bot x distance.
  Parameters:
  degrees (uint16_t): Specifies the amount of degrees to drive for.
  radius (uint16_t): Specifies the radius of the cirlce.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
// void driveCircle(uint16_t degrees, uint16_t radius, bool direction) {
//   // Calculating the encoder counts for the inner and outer wheel.
//   uint16_t distanceInnerWheel = round((float(degrees)/360) * (2*PI*(radius-7)));
//   uint16_t distanceOuterWheel = round((float(degrees)/360) * (2*PI*(radius+7)));
//   uint32_t totalInnerCount = countForDistance(distanceInnerWheel);
//   uint32_t totalOuterCount = countForDistance(distanceOuterWheel);
//   uint32_t innerEncoderCount = 0;
//   uint32_t outerEncoderCount = 0;
//
//   int8_t wheelSpeedIn = 40;  // Wheel speed for the inner wheel.
//   int8_t wheelSpeedOut = 48; // Wheel speed for the outer wheel.
//
//   // Setting wheel speed for both motors
//
//   uint8_t defaultSpeedIn = wheelSpeedIn;
//   uint8_t defaultSpeedOut = wheelSpeedOut;
//
//   float innerRatio = float(totalInnerCount)/totalOuterCount;
//   float outerRatio = float(totalOuterCount)/totalInnerCount;
//
//   // Reset encoder counts
//   resetRightEncoderCnt();
//   resetLeftEncoderCnt();
//
//   // Set up the motors
//   setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD);
//   setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);
//   enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
//   enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
//
//   // Setting speeds.
//   if(direction == LEFT){
//     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedIn);
//     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedOut);
//   } else {
//     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedOut);
//     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedIn);
//   }
//
//   // Running the motors while both are less than the total encoder count.
//   while(innerEncoderCount < totalInnerCount && outerEncoderCount < totalOuterCount){
//     // Retrieving the encoder counts. If the direction is left then inner count = left encoder count.
//     // If not then inner count = right encoder count.
//     if(direction == LEFT){
//         innerEncoderCount = getEncoderLeftCnt(); outerEncoderCount = getEncoderRightCnt();
//     } else {
//         innerEncoderCount = getEncoderRightCnt(); outerEncoderCount = getEncoderLeftCnt();
//     }
//
//     // Checking to see if the left encoder count is less than the right.
//     if((outerRatio*innerEncoderCount) + 1 < outerEncoderCount){
//       // If it is less, then check wheel speed and make sure it is under the threashold.
//       // If it is then increase the inner wheel speed. If not the set the outer wheel speed back to default.
//       if(wheelSpeedIn < 60) {
//        wheelSpeedIn++;
//       } else {
//        wheelSpeedOut = defaultSpeedOut;
//       }
//     }
//
//     // Checking to see if the right encoder count is less than the left.
//     if((innerRatio*outerEncoderCount) + 1 < innerEncoderCount){
//       // If it is left, then check wheel speed and make sure it is under the threashold.
//       // If it is then increase the outer wheel speed. If not the set the inner wheel speed back to default.
//       if(wheelSpeedOut < 70) {
//        wheelSpeedOut++;
//       } else {
//        wheelSpeedIn = defaultSpeedIn;
//       }
//     }
//
//     // Setting updated speeds.
//     if(direction == LEFT){
//       setRawMotorSpeed(LEFT_MOTOR, wheelSpeedIn);
//       setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedOut);
//     } else {
//       setRawMotorSpeed(LEFT_MOTOR, wheelSpeedOut);
//       setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedIn);
//     }
//   }
//
//   disableMotor(BOTH_MOTORS);
//   delay(500);
// }

/* Stops motors using interrupt pins.
  Returns:
  void
*/
// void stopMotorInterrupt(){
//     disableMotor(BOTH_MOTORS);
//     delay(500);
// }

/* Measures distance using ultrasonic sensor.
  Returns:
  float: Distance measured.
*/
// float measureDistanceUltra() {
//   long pulseLength;
//   float centimeters;
//   float distanceArray[11];
//
//   // Measuring 5 pulses from the ultrasonic.
//   for(size_t i=0; i < 11; i++) {
//     digitalWrite(trigPin, LOW);            // send low to get a clean pulse
//     delayMicroseconds(10);                  // let it settle
//     digitalWrite(trigPin, HIGH);           // send high to trigger device
//     delayMicroseconds(10);                 // let it settle
//     digitalWrite(trigPin, LOW);            // send low to get a clean pulse
//     delayMicroseconds(10);                  // let it settle
//     pulseLength = pulseIn(echoPin, HIGH);  // measure pulse coming back
//     centimeters = float(pulseLength) / 58;
//     distanceArray[i] = centimeters; // Adding distance to array.
//   }
//
//   // Sort the array.
//   sortArray(distanceArray);
//   delay(500);
//   return distanceArray[6]; // Returning the median of the array.
// }
//
/* Initializes the Time of Flight sensor.
  Returns:
  void
*/
void tofInit() {
    // Start i2c.
    Wire.begin();

    // Start the sensor.
    // if (distanceSensor.begin() != 0) { //Begin returns 0 on a good init
    //   Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    //   while (1);
    // }
    // Serial.println("Sensor online!");
    if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
{
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
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
float measureTOFMM() {
    int distanceArray[15];
    for(uint8_t i=0; i < 15; i++){
        distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
        while (!distanceSensor.checkForDataReady()) {
            delay(1);
          }
          distanceArray[i] = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
          distanceSensor.clearInterrupt();
          distanceSensor.stopRanging();
      }

    // Sort the 15 measurements and take the median.
    sortArray(distanceArray);
    //
    return distanceArray[8];
}

/* Measures distance in cm using the ToF sensor.
  Returns:
  float: Distance measured in cm.
*/
float measureTOFCM() {
    float distanceArray[15];
    for(uint8_t i=0; i < 15; i++){
        distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
        while (!distanceSensor.checkForDataReady()) {
            delay(1);
          }
          distanceArray[i] = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
          distanceSensor.clearInterrupt();
          distanceSensor.stopRanging();
      }

    // Sort the 15 measurements and take the median.
    sortArray(distanceArray);

    float distanceCM = distanceArray[8]/10; // Convert mm to cm.
    return distanceCM;
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

    delay(100);

    while(sensorVal[6] < 2300) {
        readLineSensor(sensorVal);
    }

    disableMotor(BOTH_MOTORS);
    delay(500);
    readLineSensor(sensorVal);
    enableMotor(BOTH_MOTORS);
    setRawMotorSpeed(RIGHT_MOTOR, 0);
    setRawMotorSpeed(LEFT_MOTOR, 0);
    setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
    setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);

    while(1) {
        readLineSensor(sensorVal);

        if(sensorVal[0] > 2300 && sensorVal[1] > 2300 && sensorVal[2] > 2300 &&
           sensorVal[3] > 2300 && sensorVal[4] > 2300 && sensorVal[5] > 2300 &&
           sensorVal[6] > 2300 && sensorVal[7] > 2300) {
            disableMotor(BOTH_MOTORS);
            delay(500);
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
