#include "HelperRSLK.h"

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

/*Turn in place the n degrees.
  Parameters:
  degrees (uint16_t): degrees to pivot.
  direction (bool): Specifies the direction of the bot.

  Returns:
  void
*/
void turnInPlace(uint16_t degrees, bool direction) {
   float distance = (float(degrees)/360)*PI*14;
   uint32_t totalEncoderCount = countForDistance(distance);
   uint8_t wheelSpeedR;
   uint8_t wheelSpeedL;

   // Reset encoder counts
   resetRightEncoderCnt();
   resetLeftEncoderCnt();

   // Turn in place left
   if(direction == LEFT){
     wheelSpeedR = 28;
     wheelSpeedL = 28;
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
     wheelSpeedL = 28;
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

/* Measures distance using ultrasonic sensor.
  Returns:
  int: Distance measured.
*/
float measureDistance() {
  long pulseLength;
  float centimeters;
  float distanceArray[11];

  // Measuring 11 pulses from the ultrasonic.
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
