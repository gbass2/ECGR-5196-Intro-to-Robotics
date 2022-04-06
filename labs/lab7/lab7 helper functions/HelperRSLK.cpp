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
  wheelSpeedL (uint16_t): The starting wheel speed for the left motor.
  wheelSpeedR (uint16_t): The starting wheel speed for the right motor.

  Returns:
  void
*/
void driveStraight(float distance, bool direction, uint8_t wheelSpeed) {
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
  resetLeftEncoderCnt();  resetRightEncoderCnt();   // Set encoder pulse count
                                                    // back to 0
  setMotorDirection(BOTH_MOTORS,direction); // Cause the robot to drive in
                                            // the direction specified
  enableMotor(BOTH_MOTORS);                         // "Turn on" the motor
  setMotorSpeed(LEFT_MOTOR,wheelSpeedL);         // Set motor speeds - variable,
  setMotorSpeed(RIGHT_MOTOR, wheelSpeedR);        //   may change (adjust) later

  // Drive both motors until both have received the correct number of pulses to travel
  while(leftTotalCount<(straight-50) || rightTotalCount<(straight-50)) {
    leftTotalCount = getEncoderLeftCnt(); rightTotalCount = getEncoderRightCnt();

    // Increasing the left wheel speed if the encoder count is less than the right.
    if(leftTotalCount + 1 < rightTotalCount){
      // If the speed of the left wheel speed is less than the defined threashold,
      // then increase the left wheel speed.
      // If it is greater than the defined threashold,
      // then reset the right wheel speed to the default.
      if(wheelSpeedL < 20) {
       wheelSpeedL++;
      } else {
       wheelSpeedR = defaultSpeedR;
      }
    }

    // Increasing the right wheel speed if the encoder count is less than the left.
    if(rightTotalCount + 1 < leftTotalCount){
      // If the speed of the right wheel speed is less than the defined threashold,
      // then increase the right wheel speed.
      // If it is greater than the defined threashold,
      // then reset the left wheel speed to the default.
      if(wheelSpeedR < 20) {
       wheelSpeedR++;
      } else {
       wheelSpeedL = defaultSpeedL;
      }
    }

    setMotorSpeed(LEFT_MOTOR, wheelSpeedL);
    setMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
  }

   // Slow down the bot with the last 50 encoder ticks
   setMotorSpeed(LEFT_MOTOR, defaultSpeedL);
   setMotorSpeed(RIGHT_MOTOR, defaultSpeedR);

   uint8_t i = 1;
   // Run the last 50 encoder ticks stepping down the speed.
   while(leftTotalCount<(straight) || rightTotalCount<(straight)) {
     leftTotalCount = getEncoderLeftCnt(); rightTotalCount = getEncoderRightCnt();

     // Step down the speed until the speed is 5.
     if(wheelSpeed-i >= 5){
      // if i is less than 6 then slow down the right motor more than the left.
       if(i <= 6){
         setMotorSpeed(LEFT_MOTOR, defaultSpeedL-i);
         setMotorSpeed(RIGHT_MOTOR, defaultSpeedR-(i+1));

       // if i is greater than 6 then step down the left motor speed more than right.
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

/*Turn in place the x degrees.
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
     wheelSpeedR = 20;
     wheelSpeedL = 21;
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_FORWARD); // Set the right motor to go forwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_BACKWARD);  // Set the left motor to go backwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                         // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount); // stay in loop
  } else {
     wheelSpeedR = 20;
     wheelSpeedL = 21;
     // Turn in place right
     // Set up the motors
     setMotorDirection(RIGHT_MOTOR,MOTOR_DIR_BACKWARD); // Set the right motor
                                                        // to go backwards
     setMotorDirection(LEFT_MOTOR,MOTOR_DIR_FORWARD);  // Set the left motor
                                                       // to go forwards
     enableMotor(RIGHT_MOTOR);                         // "Turn on" the motor
     enableMotor(LEFT_MOTOR);                          // "Turn on" the motor
     setRawMotorSpeed(RIGHT_MOTOR, wheelSpeedR);
     setRawMotorSpeed(LEFT_MOTOR, wheelSpeedL);

     // Drive motors until it has received the correct number of pulses to travel
     while(getEncoderRightCnt()<totalEncoderCount && getEncoderLeftCnt()<totalEncoderCount);       // stay in loop
  }
  disableMotor(BOTH_MOTORS);
}

/* Stops motors using interrupt pins.
  Returns:
  void
*/
void stopMotorInterrupt(){
    disableMotor(BOTH_MOTORS);
    exit(1);
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
