SparkFun Capacitive Touch Slider - CAP1203 (Qwiic)
========================================

![SparkFun Capacitive Touch Slider Board](https://cdn.sparkfun.com/assets/parts/1/3/8/7/1/15344-SparkFun_Capacitive_Touch_Slider_-_CAP1203__Qwiic_-01a.jpg)

[*SparkFun Capacitive Touch Slider - CAP1203 (Qwiic) (SEN-15344)*](https://www.sparkfun.com/products/15344)

Arduino library for the I2C based capacitive touch slider with 3 unique touch inputs. 

Repository Contents
------------------- 
* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the  library (.cpp, .h).

Advanced Configuration
----------------------
Besides touch, swipe and power button detection, the library exposes the CAP1203 tuning registers. See **Example08_AdvancedConfig**.

* **Thresholds** - `setThreshold(pad, value)`, `getThreshold(pad)`, `getDeltaCount(pad)`
* **Averaging and sampling** - `setAveragingNumberOfSamples()`, `setAveragingSamplingTime()`, `setAveragingCycleTime()` and their getters
* **Recalibration** - `setAutomaticRecalibNumberOfSamples()`, `setNegativeDeltaCounts()`, `setForceCalibrateEnabled()`, `getCountRegister()`, `getCalibrationRegister()`
* **Noise filters** - `setDIS_ANA_NOISE()`, `setDIS_RF_NOISE()` and their getters
* **Multiple touch** - `setMultiTouch()`, `getMultiTouch()`, `isLeftTouched_multi()`, `isMiddleTouched_multi()`, `isRightTouched_multi()`
* **Repeat rate** - `setRepeatRateEnabled(pad, state)`, `getRepeatRateEnabled(pad)`
* **All pads at once** - `WhichIsTouched()` returns the raw status (bit 0 = left, bit 1 = middle, bit 2 = right)

`begin()` sets the sensitivity to 16x and no longer forces the interrupt enable bits; call `setSensitivity()` and `setInterruptEnabled()` after `begin()` to get the previous behavior.

Documentation
--------------
* **[Product Repository](https://github.com/sparkfun/Qwiic_Capacitive_Touch_Slider_CAP1203)** - Product repository for the SparkFun Capacitive Touch Slider.
* **[Hookup Guide](https://learn.sparkfun.com/tutorials/capacitive-touch-slider-cap1203-hookup-guide)** - Basic hookup guide for the SparkFun Capacitive Touch Slider.

Product Versions
----------------
* [SparkFun Capacitive Touch Slider - CAP1203 (Qwiic) (SEN-15344)](https://www.sparkfun.com/products/15344) - Storefront Product

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact technical support on our [SparkFun forums](https://forum.sparkfun.com/viewforum.php?f=152).

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
