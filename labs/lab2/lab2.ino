/*
 * Class:       ECGR 4161/5196 Lab 2
 * Authors:     Grayson Bass, Ryan Roscrow
 * Date:        01-31-2022
 * Description: Blink the RGB LED in a defined sequence.
 *              Off -> red -> blue -> green -> red&green
 *              -> blue&green -> red&blue -> red&blue&green
 *              -> Off (repeat)
*/

// Defining compiler preprocessors
#define RED 75    // Define RED of the tri-color LED as pin 75
#define GREEN 76  // Define GREEN of the tri-color LED as pin 76
#define BLUE 77   // Define BLUE of the tri-color LED as pin 77

void setup() {
  // initialize one digital pin as outputs.
  pinMode(RED, OUTPUT);    //RED LED
  pinMode(GREEN, OUTPUT);  //GREEN LED
  pinMode(BLUE, OUTPUT);   //BLUE LED
}
void loop() {
  // State 1
  digitalWrite(RED, LOW);   // turn the RBG (RED) LED OFF
  digitalWrite(BLUE, LOW);  // turn the RBG (BLUE) LED OFF
  digitalWrite(GREEN, LOW); // turn the RBG (GREEN) LED OFF
  delay(500);               // wait for a half second

  // State 2
  digitalWrite(RED, HIGH); // turn the RBG (RED) LED ON
  delay(500);              // wait for a half second
  digitalWrite(RED, LOW);  // turn the RBG (RED) LED OFF

  // State 3
  digitalWrite(BLUE, HIGH); // turn the RBG (BLUE) LED ON
  delay(500);               // wait for a half second
  digitalWrite(BLUE, LOW);  // turn the RBG (BLUE) LED OFF

  // State 4
  digitalWrite(GREEN, HIGH); // turn the RBG (GREEN) LED ON
  delay(500);                // wait for a half second
  digitalWrite(GREEN, LOW);  // turn the RBG (GREEN) LED OFF

  // State 5
  digitalWrite(RED, HIGH);   // turn the RBG (RED) LED ONcan
  digitalWrite(GREEN, HIGH); // turn the RBG (GREEN) LED ON
  delay(500);                 // wait for a half second
  digitalWrite(RED, LOW);    // turn the RBG (RED) LED OFF
  digitalWrite(GREEN, LOW);  // turn the RBG (GREEN) LED OFF

  // State 6
  digitalWrite(BLUE, HIGH);  // turn the RBG (BLUE) LED ON
  digitalWrite(GREEN, HIGH); // turn the RBG (GREEN) LED ON
  delay(500);                 // wait for a half second
  digitalWrite(BLUE, LOW);   // turn the RBG (BLUE) LED OFF
  digitalWrite(GREEN, LOW);  // turn the RBG (GREEN) LED OFF

  // State 7
  digitalWrite(RED, HIGH);  // turn the RBG (RED) LED ON
  digitalWrite(BLUE, HIGH); // turn the RBG (BLUE) LED ON
  delay(500);                // wait for a half second
  digitalWrite(RED, LOW);   // turn the RBG (RED) LED OFF
  digitalWrite(BLUE, LOW);  // turn the RBG (BLUE) LED OFF

  // State 8
  digitalWrite(RED, HIGH);   // turn the RBG (RED) LED ON
  digitalWrite(BLUE, HIGH);  // turn the RBG (BLUE) LED ON
  digitalWrite(GREEN, HIGH); // turn the RBG (GREEN) LED ON
  delay(500);                 // wait for a half second
  digitalWrite(RED, LOW);    // turn the RBG (RED) LED OFF
  digitalWrite(BLUE, LOW);   // turn the RBG (BLUE) LED OFF
  digitalWrite(GREEN, LOW);  // turn the RBG (GREEN) LED OFF
}
