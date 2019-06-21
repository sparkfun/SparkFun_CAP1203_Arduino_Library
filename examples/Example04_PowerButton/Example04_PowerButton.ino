/*
  Library for the CAP1203
  By: Andrea DeVore
  SparkFun Electronics

  Do you like this library? Help support SparkFun. Buy a board!

  This sketch uses the SparkFun_CAP1203 library to initialize
  the capacitive touch sensor, enable the power button feature,
  and to detect when the power button has been pressed. This
  sketch also allows you to choose which pad acts as the power
  button and to customize how long the power button must be
  pressed before its triggered. Note: the pad which is set to
  be the power button will only act as a power button.

  License: This code is public domain, but if you see me
  (or any other SparkFun employee) at the local, and you've
  found our code helpful, please buy us a round (Beerware
  license).

  Distributed as is; no warrenty given.
*/

#include <Wire.h>
#include "SparkFun_CAP1203.h" // Click here to get the library: http://librarymanager/All#SparkFun_CAP1203

CAP1203 sensor; // Initialize sensor

void setup()
{
  Wire.begin();       // Join I2C bus
  Serial.begin(9600); // Start serial for output

  // Setup sensor
  if (sensor.begin() == false)
  {
    Serial.println("Not connected. Please check connections and read the hookup guide.");
    while (1)
      ;
  }
  else
  {
    Serial.println("Connected!");
  }

  /* SET POWER BUTTON PAD
      Set which pad acts as the power button: PAD_LEFT,PAD_MIDDLE,PAD_RIGHT
  */
  //sensor.setPowerButtonPad(PAD_LEFT);
  sensor.setPowerButtonPad(PAD_MIDDLE);
  //sensor.setPowerButtonPad(PAD_RIGHT);

  /* GET POWER BUTTON PAD
    Get the current pad which acts as the power button.
    The function returns the pad as an interger which
    maps to a specific position.

    PAD   POSITION
    1     Left
    2     Middle
    3     Right
  */
  int pad = sensor.getPowerButtonPad();
  Serial.print("Power Button Pad: ");
  if (pad == 1)
  {
    Serial.println("Left");
  }
  else if (pad == 2)
  {
    Serial.println("Middle");
  }
  if (pad == 3)
  {
    Serial.println("Right");
  }

  /* SET POWER BUTTON TIME
      Set the length of time (in ms) which the designated
      power button must indicate a touch before being
      recongized as a power button touch.
  */
  //sensor.setPowerButtonTime(PWR_TIME_280_MS);    // 280 ms
  //sensor.setPowerButtonTime(PWR_TIME_560_MS);    // 560 ms
  //sensor.setPowerButtonTime(PWR_TIME_1120_MS);   // 1120 ms
  sensor.setPowerButtonTime(PWR_TIME_2240_MS); // 2240 ms

  /* GET POWER BUTTON TIME
      Get the length of time the designated power button
      must indicate a touch before being recongized as a
      power button touch.
  */
  Serial.print("Power Button Time: ");
  Serial.print(sensor.getPowerButtonTime());
  Serial.println(" ms");

  /* SET ENABLE / DISABLE POWER BUTTON
      Enable or disable the power button functionality.
      When the power button is ENABLED, the specified pad
      (from setPowerButtonPad() function) acts as a power
      button. When the power button is DISABLED, all pads
      act as regular capactitive touch sensors.
  */
  sensor.setPowerButtonEnabled(); // Enable power button
  //sensor.setPowerButtonDisabled();  // Disable power button

  /* IS POWER BUTTON TOUCHED
      Returns the state of the power button. Returns true
      if ENABLED, otherwise returns false.
  */
  Serial.print("Power Button: ");
  if (sensor.isPowerButtonEnabled() == true)
  {
    Serial.println("ENABLED");
  }
  else
  {
    Serial.println("DISABLED");
  }
}

void loop()
{
  if (sensor.isPowerButtonTouched() == true)
  {
    Serial.println("Power Button");
    while (sensor.isPowerButtonTouched() == true)
      ; // Wait until user removes finger
  }
}
