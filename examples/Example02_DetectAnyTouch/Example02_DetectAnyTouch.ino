/*
  Library for the CAP1203
  By: Andrea DeVore
  SparkFun Electronics

  Do you like this library? Help support SparkFun. Buy a board!

  This sketch uses the SparkFun_CAP1203 library to initialize
  the capacitive touch sensor and stream when any pad detects
  a touch.

  License: This code is public domain, but if you see me
  (or any other SparkFun employee) at the local, and you've
  found our code helpful, please buy us a round (Beerware
  license).

  Distributed as is; no warrenty given.
*/

#include <SparkFun_CAP1203_Registers.h>
#include <SparkFun_CAP1203_Types.h>
#include <Wire.h>

CAP1203 sensor;     // Initialize sensor

void setup() {
  Wire.begin();         // Join I2C bus
  Serial.begin(9600);   // Start serial for output

  // Setup sensor
  if (sensor.begin() == false) {
    Serial.println("Not connected. Please check connections and read the hookup guide.");
    while (1);
  }
  else {
    Serial.println("Connected!");
  }
}

void loop() {
  if (sensor.isTouched() == true) {
    Serial.println("Touch");
  }
}
