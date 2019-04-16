#include <SparkFun_CAP1203_Registers.h>
#include <SparkFun_CAP1203_Types.h>

#include <Wire.h>

CAP1203 sensor;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  /* BEGIN
     Initializes sensor
  */
  if (sensor.begin() == false) {
    Serial.println("Not connected. Please check connections and read the hookup guide.");
    while (1);
  } else {
    Serial.println("Connected!");
  }

  /* SET SENSITVITY
     Takes in uint8_t sensitivity multiplier: 1, 2, 4, 8, 16, 32, 64, or 128
     1 is the least sensitive and 128 is the most sensitive
     2 is the default sensitivity for the SparkFun CAP1203
     Capacitive Touch Slider
  */
  sensor.setSensitivity(SENSITIVITY_2X);
  Serial.print("Sensitivity: ");
  Serial.print(sensor.getSensitivity());
  Serial.println("x");

  /* SET INTERRUPT ENABLED/DISABLED
      Enabled Interrupts so alert LED turns on
  */
   sensor.setInterruptEnabled();

  /* SET INTERRUPT ENABLED/DISABLED
    Enabled Interrupts so alert LED turns on
  */
  //sensor.setInterruptDisabled();

  /* IS INTERRUPT ENABLED
      Returns the state of interrupt pin
  */
  if (sensor.isInterruptEnabled() == true) {
    Serial.println("Interrupt ENABLED");
  }
  else {
    Serial.println("Interrupt DISABLED");
  }

  /* SET POWER BUTTON PAD
     Sets a specific pad to act as a power button
     Function takes in integer values of 1, 2, or 3
     which correlate to pad numbers respectively.

     PAD    POSTION
     1      Left
     2      Middle
     3      Right
  */
  if (sensor.setPowerButtonPad(2) == false) {
    Serial.println("Invalid pad input - setPowerButtonPad() function only takes in an integer value of 1, 2, or 3.");
    while (1);
  }
  Serial.print("Power button pad number: ");
  Serial.println(sensor.getPowerButtonPad());

  /* SET POWER BUTTON TIME
     Function takes in interger values of 280, 560, 1120, or 2240
     which correlate to the length of time (in milliseconds) pad
     must be held before recognized as a power button.
  */
  //sensor.setPowerButtonTime(PWR_TIME_280_MS);
  //sensor.setPowerButtonTime(PWR_TIME_560_MS);
 // sensor.setPowerButtonTime(PWR_TIME_2240_MS);
  
  if (sensor.setPowerButtonTime(2240) == false) {
    Serial.println("Invalid time input - setPowerButtonTime() function only takes in an integer value of 280, 560, 1120, or 2240.");
    while (1);
  }
  Serial.print("Power button hold time: ");
  Serial.print(sensor.getPowerButtonTime());
  Serial.println(" ms");

  /* SET POWER BUTTON ON
     Enable power button functionanality in active state
  */
  //sensor.setPowerButtonEnabled();

  /* SET POWER BUTTON OFF
     Diable power button functionanality in active state
  */
  sensor.setPowerButtonDisabled();

  /* IS POWER BUTTON ENABLED
     Check if power button is currently enabled (ON) ir
     disabled (OFF).
  */
  if (sensor.isPowerButtonEnabled() == true) {
    Serial.println("Power button ENABLED");
  }
  else {
    Serial.println("Power button DISABLED");
  }
}

void loop()
{
  /* IS POWER BUTTON TOUCHED
     Power button held for designated time
  */
  //  if (sensor.isPowerButtonTouched() == true) {
  //    Serial.println("Power button touched!");
  //    while (sensor.isPowerButtonTouched() == true ); // Wait for user to remove finger
  //  }

  /* IS TOUCHED
     Touch detected on any of 3 pads
  */
//  if (sensor.isTouched() == true) {
//    Serial.println("Touched");
//    while (sensor.isTouched() == true ); // Wait for user to remove finger
//  }

  /* INDIVIDUAL TOUCH PADS
     Touch detected on left, right, or middle pad
  */
//      if (sensor.isLeftTouched() == true) {
//        Serial.println("Left Touched");
//        while (sensor.isLeftTouched() == true ); // Wait for user to remove finger
//      }
//  
//      if (sensor.isMiddleTouched() == true) {
//        Serial.println("Middle Touched");
//        while (sensor.isMiddleTouched() == true ); // Wait for user to remove finger;
//      }
//  
//      if (sensor.isRightTouched() == true) {
//        Serial.println("Right Touched!");
//        while (sensor.isRightTouched() == true ); // Wait for user to remove finger
//    }


    // Check direction of swipe
    if (sensor.isRightSwipePulled() == true) {
      Serial.println("Right Swipe");
    }
    if (sensor.isLeftSwipePulled() == true) {
      Serial.println("Left Swipe");
    }
}
