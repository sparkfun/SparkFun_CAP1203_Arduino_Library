/*
  Library for the CAP1203
  By: Andrea DeVore
  SparkFun Electronics

  Do you like this library? Help support SparkFun. Buy a board!

  This sketch uses the SparkFun_CAP1203 library to initialize
  the capacitive touch sensor and set the sensitivity for
  different applications. The default sensitivity is set to
  2x for the CAP1203 Touch Slider.

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

  /* SET SENSITIVITY
      Set the sensitivity multiplier for different applications,
      such as different sized touch pads. The default sensitivity 
      multiplier is set to 2x.

      Different sensitivity settings:
      SENSITIVITY_128X  // Most sensitive
      SENSITIVITY_64X
      SENSITIVITY_32X
      SENSITIVITY_16X
      SENSITIVITY_8X
      SENSITIVITY_4X
      SENSITIVITY_2X
      SENSITIVITY_1X    // Least sensitive
  */
  sensor.setSensitivity(SENSITIVITY_4X);

  /* GET SENSITIVITY
      Returns the sensitivty multiplier for the
      current sensitivity settings.
  */
  Serial.print("Sensitivity Multiplier: ");
  Serial.print(sensor.getSensitivity());
  Serial.println("x");
}

void loop()
{
  if (sensor.isTouched() == true)
  {
    Serial.println("Touched!");
    while (sensor.isTouched() == true)
      ;
  }
}
