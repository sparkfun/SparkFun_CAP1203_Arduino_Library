/*
  Library for the CAP1203
  By: DennisMart

  This sketch uses the SparkFun_CAP1203 library to tune the
  capacitive touch sensor with the advanced configuration
  functions: per-pad thresholds, averaging and sampling,
  automatic recalibration, noise filters and multiple touch.
  It then prints the delta count of each pad and which pads
  are touched.

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

  /* TOUCH THRESHOLD
      A touch is detected when the delta count of a pad
      exceeds its threshold (power-on default is 64).
      Pads: PAD_LEFT, PAD_MIDDLE, PAD_RIGHT
  */
  sensor.setThreshold(PAD_LEFT, 64);
  sensor.setThreshold(PAD_MIDDLE, 64);
  sensor.setThreshold(PAD_RIGHT, 64);

  /* AVERAGING AND SAMPLING
      Number of samples averaged: 1, 2, 4, 8, 16, 32, 64, 128
      Sampling time (us): 320, 640, 1280, 2560
      Cycle time (ms): 35, 70, 105, 140
  */
  sensor.setAveragingNumberOfSamples(8);
  sensor.setAveragingSamplingTime(1280);
  sensor.setAveragingCycleTime(70);

  /* AUTOMATIC RECALIBRATION
      (samples, update time): (16,16), (32,32), (64,64),
      (128,128), (256,256), (256,1024), (256,2048), (256,4096)
      Negative delta counts before recalibration: 8, 16, 32, 0 (none)
  */
  sensor.setAutomaticRecalibNumberOfSamples(64, 64);
  sensor.setNegativeDeltaCounts(16);

  /* NOISE FILTERS
      false keeps the filter enabled (default).
  */
  sensor.setDIS_ANA_NOISE(false);
  sensor.setDIS_RF_NOISE(false);

  /* MULTIPLE TOUCH
      Simultaneous touches allowed: 1, 2 or 3
  */
  sensor.setMultiTouch(3);

  /* REPEAT RATE
      When enabled, a held touch keeps generating interrupts.
  */
  sensor.setRepeatRateEnabled(PAD_LEFT, true);
  sensor.setRepeatRateEnabled(PAD_MIDDLE, true);
  sensor.setRepeatRateEnabled(PAD_RIGHT, true);

  // Calibrate all pads with the new settings
  sensor.setForceCalibrateEnabled();
  delay(100);

  // Print back the configuration
  Serial.print("Thresholds: ");
  Serial.print(sensor.getThreshold(PAD_LEFT));
  Serial.print(", ");
  Serial.print(sensor.getThreshold(PAD_MIDDLE));
  Serial.print(", ");
  Serial.println(sensor.getThreshold(PAD_RIGHT));

  Serial.print("Averaging: ");
  Serial.print(sensor.getAveragingNumberOfSamples());
  Serial.print(" samples, ");
  Serial.print(sensor.getAveragingSamplingTime());
  Serial.print(" us sampling, ");
  Serial.print(sensor.getAveragingCycleTime());
  Serial.println(" ms cycle");

  uint16_t samples, updateTime;
  sensor.getAutomaticRecalibNumberOfSamples(samples, updateTime);
  Serial.print("Recalibration: ");
  Serial.print(samples);
  Serial.print(" samples, update time ");
  Serial.print(updateTime);
  Serial.print(", negative delta count ");
  Serial.println(sensor.getNegativeDeltaCounts());
}

void loop()
{
  /* DELTA COUNT
      Difference between the current measurement and the
      base count. Useful to choose the thresholds.
  */
  Serial.print("Delta L/M/R: ");
  Serial.print(sensor.getDeltaCount(PAD_LEFT));
  Serial.print("\t");
  Serial.print(sensor.getDeltaCount(PAD_MIDDLE));
  Serial.print("\t");
  Serial.print(sensor.getDeltaCount(PAD_RIGHT));

  /* WHICH IS TOUCHED
      Reads all pads at once: bit 0 = left,
      bit 1 = middle, bit 2 = right.
  */
  uint8_t touched = sensor.WhichIsTouched();
  Serial.print("\tTouched: ");
  if (touched & 0x01)
    Serial.print("Left ");
  if (touched & 0x02)
    Serial.print("Middle ");
  if (touched & 0x04)
    Serial.print("Right");
  Serial.println();

  delay(200);
}
