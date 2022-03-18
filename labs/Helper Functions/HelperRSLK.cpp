#include "HelperRSLK.h"

/*Returns the encoder pulses to trave x distance.
  Parameters:
  distance (uint32_t): distance to travel.

  Returns:
  int: encoder pulses to drive x distance.
  */
uint32_t countForDistance(uint32_t distance) {
  float temp = (WHEEL_DIAMETER * PI) / CNT_PER_REV;
  temp = distance / temp;
  return int(temp);
}

/*Drives the RSLK bot x distance.
  Parameters:
  distance (uint32_t): distance to travel.
  direction (bool): Specifies the direction of the bot.
  wheelSpeedL (uint16_t): The starting wheel speed for the left motor.
  wheelSpeedR (uint16_t): The starting wheel speed for the right motor.

  Returns:
  void
*/
void driveStraight(uint32_t distance, bool direction, uint8_t wheelSpeed) {
  // Define speed and encoder count variables
  uint8_t wheelSpeedL = wheelSpeed;
  uint8_t wheelSpeedR = wheelSpeed;
  const uint8_t defaultSpeedL = wheelSpeedL;
  const uint8_t defaultSpeedR = wheelSpeedR;
  uint16_t leftTotalCount = 0;
  uint16_t rightTotalCount = 0;

  // Amount of encoder pulses needed to achieve distance
  uint32_t straight = countForDistance(distance);

  // Set up the motors and encoders
  resetLeftEncoderCnt();  resetRightEncoderCnt();   // Set encoder pulse count back to 0
  setMotorDirection(BOTH_MOTORS,direction); // Cause the robot to drive in the direction specified
  enableMotor(BOTH_MOTORS);                         // "Turn on" the motor
  setMotorSpeed(LEFT_MOTOR,wheelSpeedL);         // Set motor speeds - variable,
  setMotorSpeed(RIGHT_MOTOR, wheelSpeedR);        //   may change (adjust) later

  // Drive both motors until both have received the correct number of pulses to travel
  while(leftTotalCount<(straight-100) || rightTotalCount<(straight-50)) {
    leftTotalCount = getEncoderLeftCnt(); rightTotalCount = getEncoderRightCnt();

    // Increasing the left wheel speed if the encoder count is less than the right.
    if(leftTotalCount + 1 < rightTotalCount){
      // If the speed of the left wheel speed is less than the defined threashold, then increase the left wheel speed.
      // If it is greater than the defined threashold, then reset the right wheel speed to the default.
      if(wheelSpeedL < 20) {
       wheelSpeedL++;
      } else {
       wheelSpeedR = defaultSpeedR;
      }
    }

    // Increasing the right wheel speed if the encoder count is less than the leftt.
    if(rightTotalCount + 1 < leftTotalCount){
      // If the speed of the right wheel speed is less than the defined threashold, then increase the right wheel speed.
      // If it is greater than the defined threashold, then reset the left wheel speed to the default.
      if(wheelSpeedR < 20) {
       wheelSpeedR++;
      } else {
       wheelSpeedL = defaultSpeedL;
      }
    }

    setMotorSpeed(LEFT_MOTOR, wheelSpeedL);
    setMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
  }

  setMotorSpeed(LEFT_MOTOR, defaultSpeedL);
  setMotorSpeed(RIGHT_MOTOR, defaultSpeedR);

  uint8_t i = 1;
  while(leftTotalCount<(straight) || rightTotalCount<(straight)) {
    leftTotalCount = getEncoderLeftCnt(); rightTotalCount = getEncoderRightCnt();

    if(wheelSpeed-i >= 5){
      if(i <= 5){
        setMotorSpeed(LEFT_MOTOR, defaultSpeedL-i);
        setMotorSpeed(RIGHT_MOTOR, defaultSpeedR-(i+1));
      } else {
        setMotorSpeed(LEFT_MOTOR, defaultSpeedL-(i+1));
        setMotorSpeed(RIGHT_MOTOR, defaultSpeedR-i);
      }

      i++;
      delay(100);
      }
  }
  disableMotor(BOTH_MOTORS);
}

/*Pivots the x degrees.
  Parameters:
  degrees (uint16_t): degrees to pivot.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
void pivot(uint16_t degrees, bool direction) {
  // Calculating the distance to pivot based on the degrees defined.
  uint16_t distance = round((float(degrees)/360)*PI*28);
  uint32_t totalEncoderCount = countForDistance(distance);
  uint8_t wheelSpeed = 10; // Setting wheel speed

  // Reset the encoder counts
  resetRightEncoderCnt();                           // Set encoder pulse count back to 0
  resetLeftEncoderCnt();

  // Pivoting left
  if(direction == LEFT){
   // Set up the motors and encoders
   setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Cause the robot to drive forward
   enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
   setMotorSpeed(RIGHT_MOTOR, wheelSpeed);

   // Drive R motor until it has received the correct number of pulses to travel
   while((getEncoderRightCnt()<totalEncoderCount));       // stay in loop
   disableMotor(RIGHT_MOTOR);
  } else {
   // Pivot right
   // Set up the motors and encoders
   setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD); // Cause the robot to drive forward
   enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
   setMotorSpeed(LEFT_MOTOR, wheelSpeed);

   // Drive L motor until it has received the correct number of pulses to travel
   while(getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
   disableMotor(LEFT_MOTOR);
  }
}


/*Turn in place the x degrees.
  Parameters:
  degrees (uint16_t): degrees to pivot.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
void turnInPlace(uint16_t degrees, bool direction) {
   float distance = round((float(degrees)/360)*PI*14);
   uint32_t totalEncoderCount = countForDistance(distance);
   uint8_t wheelSpeedR = 20;
   uint8_t wheelSpeedL = 21;

   // Reset encoder counts
   resetRightEncoderCnt();
   resetLeftEncoderCnt();

   // Turn in place left
   if(direction == LEFT){
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_BACKWARD);  // Set the left motor to go backwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
     disableMotor(RIGHT_MOTOR);
     disableMotor(LEFT_MOTOR);
  } else {
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
     disableMotor(BOTH_MOTORS);
  }
}

/*Turn in place the smallest amount possible.
  Parameters:
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
void turnInPlaceStatic(bool direction) {
   uint32_t totalEncoderCount = 5;
   uint8_t wheelSpeedR = 20;
   uint8_t wheelSpeedL = 21;

   // Reset encoder counts
   resetRightEncoderCnt();
   resetLeftEncoderCnt();

   // Turn in place left
   if(direction == LEFT){
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_BACKWARD);  // Set the left motor to go backwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
     disableMotor(RIGHT_MOTOR);
     disableMotor(LEFT_MOTOR);
  } else {
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
     disableMotor(BOTH_MOTORS);
  }
}

/*Drives the RSLK bot x distance.
  Parameters:
  degrees (uint16_t): Specifies the amount of degrees to drive for.
  radius (uint16_t): Specifies the radius of the cirlce.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
void driveCircle(uint16_t degrees, uint16_t radius, bool direction) {
  // Calculating the encoder counts for the inner and outer wheel.
  uint16_t distanceInnerWheel = round((float(degrees)/360) * (2*PI*(radius-7)));
  uint16_t distanceOuterWheel = round((float(degrees)/360) * (2*PI*(radius+7)));
  uint32_t totalInnerCount = countForDistance(distanceInnerWheel);
  uint32_t totalOuterCount = countForDistance(distanceOuterWheel);
  uint32_t innerEncoderCount = 0;
  uint32_t outerEncoderCount = 0;

  int8_t wheelSpeedIn = 40;  // Wheel speed for the inner wheel.
  int8_t wheelSpeedOut = 48; // Wheel speed for the outer wheel.

  // Setting wheel speed for both motors

  uint8_t defaultSpeedIn = wheelSpeedIn;
  uint8_t defaultSpeedOut = wheelSpeedOut;

  float innerRatio = float(totalInnerCount)/totalOuterCount;
  float outerRatio = float(totalOuterCount)/totalInnerCount;

  // Reset encoder counts
  resetRightEncoderCnt();
  resetLeftEncoderCnt();

  // Set up the motors
  setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD);
  setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);
  enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
  enableMotor(LEFT_MOTOR);                         // "Turn on" the motor

  // Setting speeds.
  if(direction == LEFT){
    setRawMotorSpeed(LEFT_MOTOR, wheelSpeedIn);
    setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedOut);
  } else {
    setRawMotorSpeed(LEFT_MOTOR, wheelSpeedOut);
    setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedIn);
  }

  // Running the motors while both are less than the total encoder count.
  while(innerEncoderCount < totalInnerCount && outerEncoderCount < totalOuterCount){
    // Retrieving the encoder counts. If the direction is left then inner count = left encoder count.
    // If not then inner count = right encoder count.
    if(direction == LEFT){
        innerEncoderCount = getEncoderLeftCnt(); outerEncoderCount = getEncoderRightCnt();
    } else {
        innerEncoderCount = getEncoderRightCnt(); outerEncoderCount = getEncoderLeftCnt();
    }

    // Checking to see if the left encoder count is less than the right.
    if((outerRatio*innerEncoderCount) + 1 < outerEncoderCount){
      // If it is less, then check wheel speed and make sure it is under the threashold.
      // If it is then increase the inner wheel speed. If not the set the outer wheel speed back to default.
      if(wheelSpeedIn < 60) {
       wheelSpeedIn++;
      } else {
       wheelSpeedOut = defaultSpeedOut;
      }
    }

    // Checking to see if the right encoder count is less than the left.
    if((innerRatio*outerEncoderCount) + 1 < innerEncoderCount){
      // If it is left, then check wheel speed and make sure it is under the threashold.
      // If it is then increase the outer wheel speed. If not the set the inner wheel speed back to default.
      if(wheelSpeedOut < 70) {
       wheelSpeedOut++;
      } else {
       wheelSpeedIn = defaultSpeedIn;
      }
    }

    // Setting updated speeds.
    if(direction == LEFT){
      setRawMotorSpeed(LEFT_MOTOR, wheelSpeedIn);
      setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedOut);
    } else {
      setRawMotorSpeed(LEFT_MOTOR, wheelSpeedOut);
      setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedIn);
    }
  }

  disableMotor(BOTH_MOTORS);
}

/* Measures distance using ultrasonic sensor.
  Returns:
  int: Distance measured.
*/
float measureDistance() {
  long pulseLength;
  float centimeters;
  float distanceArray[11];

  // Measuring 5 pulses from the ultrasonic.
  for(size_t i=0; i < 11; i++) {
    digitalWrite(trigPin, LOW);            // send low to get a clean pulse
    delayMicroseconds(10);                  // let it settle
    digitalWrite(trigPin, HIGH);           // send high to trigger device
    delayMicroseconds(10);                 // let it settle
    digitalWrite(trigPin, LOW);            // send low to get a clean pulse
    delayMicroseconds(10);                  // let it settle
    pulseLength = pulseIn(echoPin, HIGH);  // measure pulse coming back
    centimeters = float(pulseLength) / 58;
    distanceArray[i] = centimeters; // Adding distance to array.
  }

  // Sort the array.
  sortArray(distanceArray, 11);

  return distanceArray[6]; // Returning the median of the array.
}

/*Sorts an array of any type. Pass arry by reference.
  Parameters:
  array (T): Templated array to be sorted.

  Returns:
  void
*/
template<typename T> void sortArray(T& array, size_t arrSize) {
  float hold;
  for(size_t i=0; i < arrSize; i++) {
    for(size_t j=0; j < arrSize; j++) {
      if(array[j]>array[j+1]) {
        hold=array[j];
        array[j]=array[j+1];
        array[j+1]=hold;
      }
    }
  }
}
