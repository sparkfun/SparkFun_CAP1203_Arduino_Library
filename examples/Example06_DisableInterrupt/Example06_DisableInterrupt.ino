/*
  Library for the CAP1203
  By: Andrea DeVore
  SparkFun Electronics

  Do you like this library? Help support SparkFun. Buy a board!

  This sketch uses the SparkFun_CAP1203 library to initialize
  the capacitive touch sensor and disable/enable the interrupt
  pins. Disabling the interrupt pin on the SparkFun CAP1203
  Board turns off the alert LED as well.

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

  /* SET INTERRUPT DISABLED/ENABLED
      Interrupt is ENABLED as default.
      When the interrupt pin is DISABLED, the alert LED
      does not turn on. When the interrupt pin is ENABLED,
      the alert LED turns on when a touch is detected.
  */
  sensor.setInterruptDisabled(); // Disable Interrupt
  //sensor.setInterruptEnabled();   // Enable Interrupt

  /* IS INTERRUPT ENABLED
      Check the current status of the interrupt pin. 
      Returns true if interrupt pin is enabled and 
      false if disabled.
  */
  Serial.print("Interrupt: ");
  if (sensor.isInterruptEnabled() == true)
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
  if (sensor.isTouched() == true)
  {
    Serial.println("Touched!");
    while (sensor.isTouched() == true)
      ;
  }
}
