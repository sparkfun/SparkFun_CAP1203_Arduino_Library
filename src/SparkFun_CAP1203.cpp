/******************************************************************************
SparkFun_CAP1203.cpp
SFE_Capacitive Touch Slider Library - Source File
Andrea DeVore @ SparkFun Electronics
Original Creation Date: April 4, 2019
https://github.com/sparkfun/Qwiic_Capacitive_Touch_Slider_CAP1203

This file implements all functions of the CAP1203 class.

Development environment specifics:
	IDE: Arduino 1.6.0
	Hardware Platform: Arduino Uno
	CAP1203 Breakout Version: 1.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_CAP1203_Registers.h"
#include "SparkFun_CAP1203.h"

/* CONSTRUCTOR
    This function, called when you initialize the class will write 
    the varibale address into a private variable for future use. 
    The variable address should be 0x28. 
*/
CAP1203::CAP1203(byte addr)
{
    _deviceAddress = addr;
}

/* BEGIN INITIALIZATION
    This function initalizes the CAP1203 sensor.
*/
bool CAP1203::begin(TwoWire &wirePort, uint8_t deviceAddress)
{
    // Set device address and wire port to private variable
    _deviceAddress = deviceAddress;
    _i2cPort = &wirePort;

    if (isConnected() == false)
    {
        return false;
    }
    // Read PROD_ID register
    byte prodIDValue = readRegister(PROD_ID);

    // PROD_ID should always be 0x6D
    if (prodIDValue != PROD_ID_VALUE)
    {
        return false;
    }

    setSensitivity(SENSITIVITY_16X); // Set sensitivity to 16x on startup
    // setInterruptEnabled();          // Enable INT and LED as default
    clearInterrupt();               // Clear interrupt on startup
    return true;
}

/* IS CONNECTED 
    Returns true if the I2C Device acknowledgs a connection.
    Otherwise returns false.
*/
bool CAP1203::isConnected()
{
    for (byte i = 0; i < 5; i++)
    {
        /* After inspecting with logic analyzer, the device fails
            to connect for unknown reasons. The device typically connects
            after two calls. We included a for loop to allow for 
            multiple calls to the device.
        */
        _i2cPort->beginTransmission((uint8_t)_deviceAddress);
        if (_i2cPort->endTransmission() == 0)
            return (true); //Sensor did not ACK
    }

    return (false);
}

/* CHECK MAIN CONTROL REGISTER
    Control the primary power state of the device. See data sheet
    on Main Control Register (pg. 22).
*/
void CAP1203::checkMainControl()
{
    MAIN_CONTROL_REG reg;
    reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CONTROL);
}

/* CHECK STATUS
    Checks inputs in the general status register to ensure program
    is set up correctly. See data sheet on Status Registers (pg. 23).
*/
void CAP1203::checkStatus()
{
    GENERAL_STATUS_REG reg;
    reg.GENERAL_STATUS_COMBINED = readRegister(GENERAL_STATUS);
}

/* CLEAR INTTERUPT 
    Clears the interrupt (INT) bit by writing a logic 0 to it.
    This bit must be cleared in order to detec a new capacitive
    touch input. See datasheet on Main Control Register (pg. 22).
*/
void CAP1203::clearInterrupt()
{
    MAIN_CONTROL_REG reg;
    reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CONTROL);
    reg.MAIN_CONTROL_FIELDS.INT = 0x00;
    writeRegister(MAIN_CONTROL, reg.MAIN_CONTROL_COMBINED);
}

/* DISABLE INTERRUPTS 
    This disables all the interrupts, so the alert LED will not turn on
    when a sensor is touched. Set on default in begin function See data 
    sheet on Interrupt Enable Register (pg. 33).
*/
void CAP1203::setInterruptDisabled()
{
    INTERRUPT_ENABLE_REG reg;
    reg.INTERRUPT_ENABLE_COMBINED = readRegister(INTERRUPT_ENABLE);
    reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN = 0x00;
    reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN = 0x00;
    reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN = 0x00;
    writeRegister(INTERRUPT_ENABLE, reg.INTERRUPT_ENABLE_COMBINED);
}

/* ENABLE INTERRUPTS
    This turns on all the interrupts, so the alert LED turns on when any 
    sensor is touched. See data sheet on Interrupt Enable Register (pg. 33).
*/
void CAP1203::setInterruptEnabled()
{
    INTERRUPT_ENABLE_REG reg;
    reg.INTERRUPT_ENABLE_COMBINED = readRegister(INTERRUPT_ENABLE);
    reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN = 0x01;
    reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN = 0x01;
    reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN = 0x01;
    writeRegister(INTERRUPT_ENABLE, reg.INTERRUPT_ENABLE_COMBINED);
}

/* IS INTERRUPT ENABLED
    Returns state of intterupt pin. Returns true if all interrupts enabled 
    (0x07), otherwise returns false. When the interrupts are enabled, the 
    LED on the CAP1203 Touch Slider Board turns on when it detects a touch 
    (pg. 33).
*/
bool CAP1203::isInterruptEnabled()
{
    INTERRUPT_ENABLE_REG reg;
    reg.INTERRUPT_ENABLE_COMBINED = readRegister(INTERRUPT_ENABLE);
    if (reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN == 0x01 && reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN == 0x01 && reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN == 0x01)
    {
        return true;
    }
    return false;
}

/* SET SENSITIVITY
    Sensitivity calibrated for SparkFun Capacitive Touch Slider. You may 
    want to change sensitivity settings if creating your own capacitive 
    touch pads. See datasheet on Sensitivity Control Register (pg. 25).
*/
void CAP1203::setSensitivity(uint8_t sensitivity)
{
    SENSITIVITY_CONTROL_REG reg;
    reg.SENSITIVITY_CONTROL_COMBINED = readRegister(SENSITIVITY_CONTROL);
    if (sensitivity == SENSITIVITY_128X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_128X;
    }
    else if (sensitivity == SENSITIVITY_64X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_64X;
    }
    else if (sensitivity == SENSITIVITY_32X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_32X;
    }
    else if (sensitivity == SENSITIVITY_16X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_16X;
    }
    else if (sensitivity == SENSITIVITY_8X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_8X;
    }
    else if (sensitivity == SENSITIVITY_4X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_4X;
    }
    else if (sensitivity == SENSITIVITY_1X)
    {
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_1X;
    }
    else
    {
        // Default case: calibrated for CAP1203 touch sensor
        reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_2X;
    }
    writeRegister(SENSITIVITY_CONTROL, reg.SENSITIVITY_CONTROL_COMBINED);
}

/* GET SENSITIVITY
    Returns the sensitivity multiplier for current sensitivity settings (pg. 25).
*/
uint8_t CAP1203::getSensitivity()
{
    SENSITIVITY_CONTROL_REG reg;
    reg.SENSITIVITY_CONTROL_COMBINED = readRegister(SENSITIVITY_CONTROL);
    uint16_t sensitivity = reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE;
    if (sensitivity == SENSITIVITY_128X)
    {
        return 128;
    }
    else if (sensitivity == SENSITIVITY_64X)
    {
        return 64;
    }
    else if (sensitivity == SENSITIVITY_32X)
    {
        return 32;
    }
    else if (sensitivity == SENSITIVITY_16X)
    {
        return 16;
    }
    else if (sensitivity == SENSITIVITY_8X)
    {
        return 8;
    }
    else if (sensitivity == SENSITIVITY_4X)
    {
        return 4;
    }
    else if (sensitivity == SENSITIVITY_2X)
    {
        return 2;
    }
    else if (sensitivity == SENSITIVITY_1X)
    {
        return 1;
    }
    else
    {
        // Error - no possible register value (pg. 25)
        return 0;
    }
}

/* GET MULTIPLE TOUCH CONFIGURATION
    Returns the raw Multiple Touch Configuration register (0x2A). Bits 3:2
    (B_MULT_T) hold the number of simultaneous touches allowed, bit 7
    (MULT_BLK_EN) enables multiple touch blocking.
*/
uint8_t CAP1203::getMultiTouch()
{
    MULTITOUCH_STATUS_REG reg;
    reg.MULTITOUCH_STATUS_COMBINED = readRegister(MULTIPLE_TOUCH_CONFIG);
    return reg.MULTITOUCH_STATUS_COMBINED;
}

/* GET BASE COUNT
    Returns the Base Count register (0x50-0x52) of a sensor input
    (1 = left, 2 = middle, 3 = right). Returns 0 for an invalid input.
*/
uint8_t CAP1203::getCountRegister(const uint8_t touch)
{
    uint8_t reg = 0x00;
    if (touch == 1)
    {
        reg = readRegister(SENSOR_INPUT_1_BASE_COUNT);
    }
    else if (touch == 2)
    {
        reg = readRegister(SENSOR_INPUT_2_BASE_COUNT);
    }
    else if (touch == 3)
    {
        reg = readRegister(SENSOR_INPUT_3_BASE_COUNT);
    }
    else
    {
        reg = 0x00;
    }

    return reg;
}

/* GET CALIBRATION
    Returns the Sensor Input Calibration register (0xB1-0xB3) of a sensor
    input (1 = left, 2 = middle, 3 = right). Returns 0 for an invalid input.
*/
uint8_t CAP1203::getCalibrationRegister(const uint8_t touch)
{
    uint8_t reg = 0x00;
    if (touch == 1)
    {
        reg = readRegister(SENSOR_INPUT_1_CALIBRATION);
    }
    else if (touch == 2)
    {
        reg = readRegister(SENSOR_INPUT_2_CALIBRATION);
    }
    else if (touch == 3)
    {
        reg = readRegister(SENSOR_INPUT_3_CALIBRATION);
    }
    else
    {
        reg = 0x00;
    }

    return reg;
}

/* SET MULTIPLE TOUCH
    Sets how many simultaneous touches (1, 2 or 3) are allowed before a
    multiple touch event is flagged. Writes B_MULT_T in the Multiple Touch
    Configuration register (0x2A) and returns the new register value.
    Other inputs leave the setting unchanged.
*/
uint8_t CAP1203::setMultiTouch(uint8_t MultiTouch)
{
    MULTITOUCH_STATUS_REG reg;
    reg.MULTITOUCH_STATUS_COMBINED = readRegister(MULTIPLE_TOUCH_CONFIG);
    if (MultiTouch == 1)
    {
        reg.MULTITOUCH_STATUS_FIELDS.B_MULT = 0x00;
    }
    else if (MultiTouch == 2)
    {
        reg.MULTITOUCH_STATUS_FIELDS.B_MULT = 0x01;
    }
    else if (MultiTouch == 3)
    {
        reg.MULTITOUCH_STATUS_FIELDS.B_MULT = 0x03;
    }

    writeRegister(MULTIPLE_TOUCH_CONFIG, reg.MULTITOUCH_STATUS_COMBINED);
    return reg.MULTITOUCH_STATUS_COMBINED;
}
/* LEFT SENSOR TOUCHED
    Checks if touch input detected on left sensor (pad 1). Need to clear
    interrupt pin after touch occurs. See datasheet on Sensor Interrupt 
    Status Reg (pg.23).
*/
bool CAP1203::isLeftTouched()
{
    SENSOR_INPUT_STATUS_REG reg;

    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    if (reg.SENSOR_INPUT_STATUS_FIELDS.CS1 == ON)
    {
        clearInterrupt();

        return true;
    }

    return false;
}

/* LEFT SENSOR, MULTIPLE TOUCH PATTERN
    Checks the left sensor (CS1) bit of the Multiple Touch Pattern
    register (0x2D). Clears the interrupt when the bit is set.
*/
bool CAP1203::isLeftTouched_multi()
{
    MULTIPLE_INPUT_STATUS_REG reg;
    reg.MULTIPLE_INPUT_STATUS_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN);

    // Touch detected
    if (reg.MULTIPLE_INPUT_STATUS_FIELDS.CS1 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

/* MIDDLE SENSOR TOUCHED
    Checks if touch input detected on left sensor (pad 2). Need to clear
    interrupt pin after touch occurs. See datasheet on Sensor Interrupt 
    Status Reg (pg.23).
*/
bool CAP1203::isMiddleTouched()
{
    SENSOR_INPUT_STATUS_REG reg;
    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    if (reg.SENSOR_INPUT_STATUS_FIELDS.CS2 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

/* MIDDLE SENSOR, MULTIPLE TOUCH PATTERN
    Checks the middle sensor (CS2) bit of the Multiple Touch Pattern
    register (0x2D). Clears the interrupt when the bit is set.
*/
bool CAP1203::isMiddleTouched_multi()
{
    MULTIPLE_INPUT_STATUS_REG reg;
    reg.MULTIPLE_INPUT_STATUS_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN);

    // Touch detected
    if (reg.MULTIPLE_INPUT_STATUS_FIELDS.CS2 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

/* RIGHT SENSOR TOUCHED
    Checks if touch input detected on left sensor (pad 3). Need to clear
    interrupt pin after touch occurs. See datasheet on Sensor Interrupt 
    Status Reg (pg.23).
*/
bool CAP1203::isRightTouched()
{
    SENSOR_INPUT_STATUS_REG reg;

    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    if (reg.SENSOR_INPUT_STATUS_FIELDS.CS3 == ON)
    {
        clearInterrupt();

        return true;
    }

    return false;
}

/* RIGHT SENSOR, MULTIPLE TOUCH PATTERN
    Checks the right sensor (CS3) bit of the Multiple Touch Pattern
    register (0x2D). Clears the interrupt when the bit is set.
*/
bool CAP1203::isRightTouched_multi()
{
    MULTIPLE_INPUT_STATUS_REG reg;
    reg.MULTIPLE_INPUT_STATUS_COMBINED = readRegister(MULTIPLE_TOUCH_PATTERN);

    // Touch detected
    if (reg.MULTIPLE_INPUT_STATUS_FIELDS.CS3 == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

/* DETECT TOUCH
    Checks if touch input detected on any sensor. Need to clear
    interrupt pin after touch occurs. See datasheet on Sensor Interrupt 
    Status (pg.23).
*/
bool CAP1203::isTouched()
{
    GENERAL_STATUS_REG reg;
    reg.GENERAL_STATUS_COMBINED = readRegister(GENERAL_STATUS);

    // Touch detected
    if (reg.GENERAL_STATUS_FIELDS.TOUCH == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

/* WHICH SENSOR IS TOUCHED
    Returns the raw Sensor Input Status register (0x03) so all pads can be
    checked with one read: bit 0 = left, bit 1 = middle, bit 2 = right.
    Clears the interrupt.
*/
uint8_t CAP1203::WhichIsTouched()
{
    SENSOR_INPUT_STATUS_REG reg;
    reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUT_STATUS);

    // Touch detected
    clearInterrupt();
    return reg.SENSOR_INPUT_STATUS_COMBINED;
}

/* IS RIGHT SWIPE
    Checks if a right swipe occured on the board. This method
    takes up all functionality due to implementation of 
    while loop with millis().
*/
bool CAP1203::isRightSwipePulled()
{
    bool swipe = false; // Tracks if conditions are being met
    unsigned long startTime = millis();

    // LEFT TOUCH CONDITION
    while ((millis() - startTime) < 100)
    {
        if (isLeftTouched() == true)
        {
            swipe = true;
            while (isLeftTouched() == true)
                ;  // Wait for user to remove their finger
            break; // Break out of loop
        }
    }

    // Return if left not touched
    if (swipe == false)
        return false;

    startTime = millis(); // Reset start time
    swipe = false;        // Reset check statement

    // MIDDLE TOUCH CONDITION
    while ((millis() - startTime) < 100)
    {
        if (isMiddleTouched() == true)
        {
            swipe = true;
            while (isMiddleTouched() == true)
                ;  // Wait for user to remove their finger
            break; // Break out of loop
        }
    }

    // Return if middle not touched
    if (swipe == false)
    {
        return false;
    }

    startTime = millis(); // Reset start time
    swipe = false;        // Reset check statement

    // RIGHT TOUCH CONDITION
    while ((millis() - startTime) < 100)
    {
        if (isRightTouched() == true)
        {
            return true;
        }
    }

    return false;
}

/* IS LEFT SWIPE PULLED
    Checks if a left swipe occured on the board. This method
    takes up all functionality due to implementation of 
    while loop with millis().
*/
bool CAP1203::isLeftSwipePulled()
{
    bool swipe = false; // Tracks if conditions are being met
    unsigned long startTime = millis();

    // RIGHT TOUCH CONDITION
    while ((millis() - startTime) < 100)
    {
        if (isRightTouched() == true)
        {
            swipe = true;
            while (isRightTouched() == true)
                ;  // Wait for user to remove their finger
            break; // Break out of loop
        }
    }

    // Return if right not touched
    if (swipe == false)
        return false;

    startTime = millis(); // Reset start time
    swipe = false;        // Reset check statement

    // MIDDLE TOUCH CONDITION
    while ((millis() - startTime) < 100)
    {
        if (isMiddleTouched() == true)
        {
            swipe = true;
            while (isMiddleTouched() == true)
                ;  // Wait for user to remove their finger
            break; // Break out of loop
        }
    }

    // Return if middle not touched
    if (swipe == false)
        return false;

    startTime = millis(); // Reset start time
    swipe = false;        // Reset check statement

    // LEFT TOUCH CONDITION
    while ((millis() - startTime) < 100)
    {
        if (isLeftTouched() == true)
        {
            return true;
        }
    }

    return false;
}

/* SET POWER BUTTON PAD
    Sets a specific pad to act as a power button. Function takes in which 
    pad to set as power button. See datasheet on Power Button (pg. 16).
*/
bool CAP1203::setPowerButtonPad(uint8_t pad)
{
    POWER_BUTTON_REG reg;
    reg.POWER_BUTTON_COMBINED = readRegister(POWER_BUTTON);

    // Set pad to act as power button (pg. 43)
    if (pad == PAD_LEFT)
    {
        reg.POWER_BUTTON_FIELDS.PWR_BTN = PWR_CS1;
    }
    else if (pad == PAD_MIDDLE)
    {
        reg.POWER_BUTTON_FIELDS.PWR_BTN = PWR_CS2;
    }
    else if (pad == PAD_RIGHT)
    {
        reg.POWER_BUTTON_FIELDS.PWR_BTN = PWR_CS3;
    }
    else
    {
        // User input invalid pad number
        return false;
    }
    writeRegister(POWER_BUTTON, reg.POWER_BUTTON_COMBINED);
    return true;
}

/* GET POWER BUTTON PAD
    Returns which capacitive touch pad is currently set to act
    as a power button.
    
    Add 1 to return value so value matches pad number.
    See data sheet on Power Button (pg. 44)
        REG VALUE   PAD
        0x00        1
        0x01        2
        0x02        3
*/
uint8_t CAP1203::getPowerButtonPad()
{
    POWER_BUTTON_REG reg;
    reg.POWER_BUTTON_COMBINED = readRegister(POWER_BUTTON);

    return (reg.POWER_BUTTON_FIELDS.PWR_BTN + 1);
}

/* SET POWER BUTTON TIME
    Configure the length of time that the designated power button
    must indicate a touch before an interrupt is generated and the
    power status indicator is set. See data sheet on Power Button
    Configuration Register (pg. 43). 
    
    Possible inputs (represent time in ms): 280, 560, 1120, 2240
*/
bool CAP1203::setPowerButtonTime(uint8_t inputTime)
{
    POWER_BUTTON_CONFIG_REG reg;
    reg.POWER_BUTTON_CONFIG_COMBINED = readRegister(POWER_BUTTON_CONFIG);
    if (inputTime == PWR_TIME_280_MS)
    {
        reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME = PWR_TIME_280_MS;
    }
    else if (inputTime == PWR_TIME_560_MS)
    {
        reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME = PWR_TIME_560_MS;
    }
    else if (inputTime == PWR_TIME_1120_MS)
    {
        reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME = PWR_TIME_1120_MS;
    }
    else if (inputTime == PWR_TIME_2240_MS)
    {
        reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME = PWR_TIME_2240_MS;
    }
    else
    {
        // User input invalid time
        return false;
    }
    writeRegister(POWER_BUTTON_CONFIG, reg.POWER_BUTTON_CONFIG_COMBINED);
    return true;
}

/* GET POWER BUTTON TIME
    Returns the length of the time designated time power button must 
    indicate a touch before an interrupt is generated. 
    
    See data sheet on Power Button Time (pg. 44)
        REG VALUE   TIME
        0x00        280 MS
        0x01        560 MS
        0x02        1120 MS
        0x03        2240 MS
*/
uint16_t CAP1203::getPowerButtonTime()
{
    POWER_BUTTON_CONFIG_REG reg;
    reg.POWER_BUTTON_CONFIG_COMBINED = readRegister(POWER_BUTTON_CONFIG);
    if (reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME == PWR_TIME_280_MS)
    {
        return 280;
    }
    else if (reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME == PWR_TIME_560_MS)
    {
        return 560;
    }
    else if (reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME == PWR_TIME_1120_MS)
    {
        return 1120;
    }
    else if (reg.POWER_BUTTON_CONFIG_FIELDS.PWR_TIME == PWR_TIME_2240_MS)
    {
        return 2240;
    }
    // Invalid data reading - check hook up
    return 0;
}

/* SET POWER BUTTON ENABLED
    Enables power button in active state. See data sheet on Power Button
    Configuration Register (pg. 43-44)
*/
void CAP1203::setPowerButtonEnabled()
{
    POWER_BUTTON_CONFIG_REG reg;
    reg.POWER_BUTTON_CONFIG_COMBINED = readRegister(POWER_BUTTON_CONFIG);
    reg.POWER_BUTTON_CONFIG_FIELDS.PWR_EN = 0x01;
    writeRegister(POWER_BUTTON_CONFIG, reg.POWER_BUTTON_CONFIG_COMBINED);
}

/* SET POWER BUTTON DISABLED
    Disables power button in active state. See data sheet on Power Button
    Configuration Register (pg. 43-44)
*/
void CAP1203::setPowerButtonDisabled()
{
    POWER_BUTTON_CONFIG_REG reg;
    reg.POWER_BUTTON_CONFIG_COMBINED = readRegister(POWER_BUTTON_CONFIG);
    reg.POWER_BUTTON_CONFIG_FIELDS.PWR_EN = 0x00;
    writeRegister(POWER_BUTTON_CONFIG, reg.POWER_BUTTON_CONFIG_COMBINED);
}

/* IS POWER BUTTON ENABLED
    Returns state of power button. Returns true if enabled (reg. value is
    0x01), otherwise returns false. Power button must be ENABLED to use.
    See data sheet on Power Button Configuration Register (pg. 43-44).
*/
bool CAP1203::isPowerButtonEnabled()
{
    POWER_BUTTON_CONFIG_REG reg;
    reg.POWER_BUTTON_CONFIG_COMBINED = readRegister(POWER_BUTTON_CONFIG);
    if (reg.POWER_BUTTON_CONFIG_FIELDS.PWR_EN == 0x01)
    {
        // Power button enabled
        return true;
    }
    // Power button disabled
    return false;
}

/* IS POWER BUTTON TOUCHED
    Once the power button has been held for designated time, an interrupt 
    is generated and PWR bit is set in the General Status Register. See 
    data sheet on Power Button (pg. 16), Power Button Register (pg. 43),
    and Power Button Configuration Register (pg. 43).
*/
bool CAP1203::isPowerButtonTouched()
{
    GENERAL_STATUS_REG reg;
    reg.GENERAL_STATUS_COMBINED = readRegister(GENERAL_STATUS);

    if (reg.GENERAL_STATUS_FIELDS.PWR == ON)
    {
        clearInterrupt();
        return true;
    }
    return false;
}

/* READ A SINGLE REGISTER
    Read a single byte of data from the CAP1203 register "reg"
*/
byte CAP1203::readRegister(CAP1203_Register reg)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission(false);               // endTransmission but keep the connection active
    _i2cPort->requestFrom(_deviceAddress, (byte)1); // Ask for 1 byte, once done, bus is released by default

    // Wait for the data to come back
    if (_i2cPort->available())
    {
        return _i2cPort->read(); // Return this one byte
    }
    else
    {
        return 0;
    }
}

/* READ MULTIPLE REGISTERS
    Read "en" bytes from the CAP1203m, starting at register "reg." Bytes are 
    stored in "buffer" on exit.
*/
void CAP1203::readRegisters(CAP1203_Register reg, byte *buffer, byte len)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission(false);           // endTransmission but keep the connection active
    _i2cPort->requestFrom(_deviceAddress, len); // Ask for bytes, once done, bus is released by default

    // Wait for data to come back
    if (_i2cPort->available() == len)
    {
        // Iterate through data from buffer
        for (int i = 0; i < len; i++)
            buffer[i] = _i2cPort->read();
    }
}

/* WRITE TO A SINGLE REGISTER
    Wire a single btyte of data to a register in CAP1203
*/
void CAP1203::writeRegister(CAP1203_Register reg, byte data)
{
    writeRegisters(reg, &data, 1);
}

/* WRITE TO MULTIPLE REGISTERS
    Write an array of "len" bytes ("buffer"), starting at register "reg,"
    and auto-incementing to the next
*/
void CAP1203::writeRegisters(CAP1203_Register reg, byte *buffer, byte len)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    for (int i = 0; i < len; i++)
        _i2cPort->write(buffer[i]);
    _i2cPort->endTransmission(); // Stop transmitting
}

/* GET TOUCH THRESHOLD
    Returns the Sensor Input Threshold register (0x30-0x32) of a pad
    (PAD_LEFT, PAD_MIDDLE, PAD_RIGHT). A touch is detected when the delta
    count exceeds this value. Returns 0 for an invalid pad.
*/
uint8_t CAP1203::getThreshold(uint8_t pad)
{
    uint8_t rb;
    if (pad == PAD_LEFT)
    {
        rb = readRegister(SENSOR_1_INPUT_THRESH);
    }
    else if (pad == PAD_MIDDLE)
    {
        rb = readRegister(SENSOR_2_INPUT_THRESH);
    }
    else if (pad == PAD_RIGHT)
    {
        rb = readRegister(SENSOR_3_INPUT_THRESH);
    }
    else
    {
        // User input invalid pad number
        return 0x00;
    }

    return rb;
}

/* SET TOUCH THRESHOLD
    Writes the Sensor Input Threshold register (0x30-0x32) of a pad
    (PAD_LEFT, PAD_MIDDLE, PAD_RIGHT).
*/
void CAP1203::setThreshold(uint8_t pad, uint8_t threshold)
{
    if (pad == PAD_LEFT)
    {
        writeRegister(SENSOR_1_INPUT_THRESH, threshold);
    }
    else if (pad == PAD_MIDDLE)
    {
        writeRegister(SENSOR_2_INPUT_THRESH, threshold);
    }
    else if (pad == PAD_RIGHT)
    {
        writeRegister(SENSOR_3_INPUT_THRESH, threshold);
    }
}

/* FORCE CALIBRATION
    Sets the CS1_CAL, CS2_CAL and CS3_CAL bits of the Calibration Activate
    and Status register (0x26) to start a calibration of all three inputs.
    Use getForceCalibrateEnabledState() to read the register back.
*/
void CAP1203::setForceCalibrateEnabled()
{
    CALIBRATION_ACTIVATE_AND_STATUS_REG reg;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED = readRegister(CALIBRATION_ACTIVATE_AND_STATUS);

    reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS1_CAL = 0x01;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS2_CAL = 0x01;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS3_CAL = 0x01;

    writeRegister(CALIBRATION_ACTIVATE_AND_STATUS, reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED);
}

/* GET CALIBRATION STATUS
    Returns the raw Calibration Activate and Status register (0x26).
*/
uint8_t CAP1203::getForceCalibrateEnabledState()
{
    CALIBRATION_ACTIVATE_AND_STATUS_REG reg;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED = readRegister(CALIBRATION_ACTIVATE_AND_STATUS);

    return reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED;
}

/* CLEAR FORCED CALIBRATION
    Clears the CS1_CAL, CS2_CAL and CS3_CAL bits of the Calibration
    Activate and Status register (0x26).
*/
void CAP1203::setForceCalibrateDisabled()
{
    CALIBRATION_ACTIVATE_AND_STATUS_REG reg;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED = readRegister(CALIBRATION_ACTIVATE_AND_STATUS);

    reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS1_CAL = 0x00;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS2_CAL = 0x00;
    reg.CALIBRATION_ACTIVATE_AND_STATUS_FIELDS.CS3_CAL = 0x00;

    writeRegister(CALIBRATION_ACTIVATE_AND_STATUS, reg.CALIBRATION_ACTIVATE_AND_STATUS_COMBINED);
}

/* GET DELTA COUNT
    Returns the signed Sensor Input Delta Count (0x10-0x12) of a pad
    (PAD_LEFT, PAD_MIDDLE, PAD_RIGHT): the difference between the current
    measurement and the base count. Returns 0 for an invalid pad.
*/
int8_t CAP1203::getDeltaCount(uint8_t pad)
{
    int8_t rb;
    if (pad == PAD_LEFT)
    {
        rb = readRegister(SENSOR_INPUT_1_DELTA_COUNT);
    }
    else if (pad == PAD_MIDDLE)
    {
        rb = readRegister(SENSOR_INPUT_2_DELTA_COUNT);
    }
    else if (pad == PAD_RIGHT)
    {
        rb = readRegister(SENSOR_INPUT_3_DELTA_COUNT);
    }
    else
    {
        // User input invalid pad number
        return 0x00;
    }
    return rb;
}

/* GET REPEAT RATE ENABLED
    Returns the Repeat Rate Enable bit (0x28) of a pad (PAD_LEFT,
    PAD_MIDDLE, PAD_RIGHT). Returns false for an invalid pad.
*/
bool CAP1203::getRepeatRateEnabled(uint8_t pad)
{
    uint8_t reg_value = readRegister(REPEAT_RATE_ENABLE);
    bool state = false;

    switch (pad)
    {
    case PAD_LEFT:
        state = (reg_value & 0x01) != 0;
        break;
    case PAD_MIDDLE:
        state = (reg_value & 0x02) != 0;
        break;
    case PAD_RIGHT:
        state = (reg_value & 0x04) != 0;
        break;
    default:
        // Handle invalid pad number
        break;
    }

    return state;
}

/* SET REPEAT RATE ENABLED
    Sets or clears the Repeat Rate Enable bit (0x28) of a pad. When
    enabled, a held touch keeps generating interrupts.
*/
void CAP1203::setRepeatRateEnabled(uint8_t pad, bool state)
{
    uint8_t org = readRegister(REPEAT_RATE_ENABLE);

    if (pad == PAD_LEFT)
    {
        if (state)
        {
            writeRegister(REPEAT_RATE_ENABLE, (org | 0x01));
        }
        else
        {
            writeRegister(REPEAT_RATE_ENABLE, (org & 0x06));
        }
    }
    else if (pad == PAD_MIDDLE)
    {
        if (state)
        {
            writeRegister(REPEAT_RATE_ENABLE, (org | 0x02));
        }
        else
        {
            writeRegister(REPEAT_RATE_ENABLE, (org & 0x05));
        }
    }
    else if (pad == PAD_RIGHT)
    {
        if (state)
        {
            writeRegister(REPEAT_RATE_ENABLE, (org | 0x04));
        }
        else
        {
            writeRegister(REPEAT_RATE_ENABLE, (org & 0x03));
        }
    }
}

/* ANALOG NOISE FILTER
    Returns DIS_ANA_NOISE (bit 4) of the Configuration register (0x20).
    true means the analog noise filter is disabled.
*/
bool CAP1203::getDIS_ANA_NOISE()
{
    uint8_t reg_value = readRegister(CONFIG);
    return (reg_value & 0x10) != 0;
}

/* ANALOG NOISE FILTER
    Sets DIS_ANA_NOISE (bit 4) of the Configuration register (0x20).
    true disables the analog noise filter.
*/
void CAP1203::setDIS_ANA_NOISE(bool state)
{
    uint8_t org = readRegister(CONFIG);
    if (state)
    {
        writeRegister(CONFIG, (org | 0x10));
    }
    else
    {
        writeRegister(CONFIG, (org & 0xEF));
    }
}

/* RF NOISE FILTER
    Sets DIS_RF_NOISE (bit 2) of the Configuration 2 register (0x44).
    true disables RF noise detection.
*/
void CAP1203::setDIS_RF_NOISE(bool state)
{
    uint8_t org = readRegister(CONFIG_2);
    if (state)
    {
        writeRegister(CONFIG_2, (org | 0x04));
    }
    else
    {
        writeRegister(CONFIG_2, (org & 0xFB));
    }
}

/* RF NOISE FILTER
    Returns DIS_RF_NOISE (bit 2) of the Configuration 2 register (0x44).
*/
bool CAP1203::getDIS_RF_NOISE()
{
    uint8_t reg_value = readRegister(CONFIG_2);
    return (reg_value & 0x04) != 0;
}

/* AUTOMATIC RECALIBRATION
    Sets CAL_CFG (bits 2:0) of the Recalibration Configuration register
    (0x2F): samples averaged and update time of automatic recalibration.
    Valid pairs: (16,16), (32,32), (64,64), (128,128), (256,256),
    (256,1024), (256,2048), (256,4096). Other pairs set (64,64).
*/
void CAP1203::setAutomaticRecalibNumberOfSamples(uint16_t num_samples, uint16_t update_time)
{
    uint8_t org = readRegister(RECALIBRATION_CONFIG);
    uint8_t value;
    if ((num_samples == 16) && (update_time == 16))
    {
        value = 0x00;
    }
    else if ((num_samples == 32) && (update_time == 32))
    {
        value = 0x01;
    }
    else if ((num_samples == 64) && (update_time == 64))
    {
        value = 0x02;
    }
    else if ((num_samples == 128) && (update_time == 128))
    {
        value = 0x03;
    }
    else if ((num_samples == 256) && (update_time == 256))
    {
        value = 0x04;
    }
    else if ((num_samples == 256) && (update_time == 1024))
    {
        value = 0x05;
    }
    else if ((num_samples == 256) && (update_time == 2048))
    {
        value = 0x06;
    }
    else if ((num_samples == 256) && (update_time == 4096))
    {
        value = 0x07;
    }
    else
    {
        value = 0x02; // default: (num_samples, update_time) = (64, 64)
    }
    writeRegister(RECALIBRATION_CONFIG, (org & 0xF8) | value);
}

/* AUTOMATIC RECALIBRATION
    Reads CAL_CFG (bits 2:0) of the Recalibration Configuration register
    (0x2F) into num_samples and update_time.
*/
void CAP1203::getAutomaticRecalibNumberOfSamples(uint16_t &num_samples, uint16_t &update_time)
{
    uint8_t reg_value = readRegister(RECALIBRATION_CONFIG);

    uint8_t config = reg_value & 0x07; // CAL_CFG, bits 2:0

    switch (config)
    {
    case 0x00:
        num_samples = 16;
        update_time = 16;
        break;
    case 0x01:
        num_samples = 32;
        update_time = 32;
        break;
    case 0x02:
        num_samples = 64;
        update_time = 64;
        break;
    case 0x03:
        num_samples = 128;
        update_time = 128;
        break;
    case 0x04:
        num_samples = 256;
        update_time = 256;
        break;
    case 0x05:
        num_samples = 256;
        update_time = 1024;
        break;
    case 0x06:
        num_samples = 256;
        update_time = 2048;
        break;
    case 0x07:
        num_samples = 256;
        update_time = 4096;
        break;
    default:
        num_samples = 64; // Default value
        update_time = 64; // Default value
        break;
    }
}

/* NEGATIVE DELTA COUNT
    Sets NEG_DELTA_CNT (bits 4:3) of the Recalibration Configuration
    register (0x2F): consecutive negative delta counts that trigger a
    recalibration. Valid values: 8, 16, 32, 0 (none). Others set 16.
*/
void CAP1203::setNegativeDeltaCounts(uint8_t counts)
{
    uint8_t org = readRegister(RECALIBRATION_CONFIG);
    uint8_t value;
    if (counts == 8)
    {
        value = 0x00;
    }
    else if (counts == 16)
    {
        value = 0x08;
    }
    else if (counts == 32)
    {
        value = 0x10;
    }
    else if (counts == 0)
    {
        value = 0x18;
    }
    else
    {
        value = 0x08; // default: 16
    }
    writeRegister(RECALIBRATION_CONFIG, (org & 0xE7) | value);
}

/* NEGATIVE DELTA COUNT
    Returns NEG_DELTA_CNT of the Recalibration Configuration register
    (0x2F): 8, 16, 32 or 0 (none).
*/
uint8_t CAP1203::getNegativeDeltaCounts()
{
    uint8_t reg_value = readRegister(RECALIBRATION_CONFIG);

    uint8_t counts_bits = reg_value & 0x18; // NEG_DELTA_CNT, bits 4:3

    switch (counts_bits)
    {
    case 0x00:
        return 8;
    case 0x08:
        return 16;
    case 0x10:
        return 32;
    case 0x18:
        return 0;
    default:
        return 16; // Default value
    }
}

/* CYCLE TIME
    Sets CYCLE_TIME (bits 1:0) of the Averaging and Sampling Configuration
    register (0x24) in ms. Valid values: 35, 70, 105, 140. Others set 70.
*/
void CAP1203::setAveragingCycleTime(uint8_t cycle_time_ms)
{
    uint8_t org = readRegister(AVERAGING_AND_SAMPLE_CONFIG);
    uint8_t value;
    if (cycle_time_ms == 35)
    {
        value = 0x00;
    }
    else if (cycle_time_ms == 70)
    {
        value = 0x01;
    }
    else if (cycle_time_ms == 105)
    {
        value = 0x02;
    }
    else if (cycle_time_ms == 140)
    {
        value = 0x03;
    }
    else
    {
        value = 0x01; // default: 70msec
    }
    writeRegister(AVERAGING_AND_SAMPLE_CONFIG, (org & 0xFC) | value);
}

/* CYCLE TIME
    Returns CYCLE_TIME of the Averaging and Sampling Configuration
    register (0x24) in ms.
*/
uint8_t CAP1203::getAveragingCycleTime()
{
    uint8_t reg_value = readRegister(AVERAGING_AND_SAMPLE_CONFIG);

    uint8_t cycle_time_bits = reg_value & 0x03; // CYCLE_TIME, bits 1:0

    switch (cycle_time_bits)
    {
    case 0x00:
        return 35;
    case 0x01:
        return 70;
    case 0x02:
        return 105;
    case 0x03:
        return 140;
    default:
        return 70; // Default value
    }
}

/* SAMPLING TIME
    Sets SAMP_TIME (bits 3:2) of the Averaging and Sampling Configuration
    register (0x24) in us. Valid values: 320, 640, 1280, 2560. Others set
    1280.
*/
void CAP1203::setAveragingSamplingTime(uint16_t samp_time_us)
{
    uint8_t org = readRegister(AVERAGING_AND_SAMPLE_CONFIG);
    uint8_t value;
    if (samp_time_us == 320)
    {
        value = 0x00;
    }
    else if (samp_time_us == 640)
    {
        value = 0x04;
    }
    else if (samp_time_us == 1280)
    {
        value = 0x08;
    }
    else if (samp_time_us == 2560)
    {
        value = 0x0C;
    }
    else
    {
        value = 0x08; // default: 1280us = 1.28msec
    }
    writeRegister(AVERAGING_AND_SAMPLE_CONFIG, (org & 0xF3) | value);
}

/* SAMPLING TIME
    Returns SAMP_TIME of the Averaging and Sampling Configuration register
    (0x24) in us.
*/
uint16_t CAP1203::getAveragingSamplingTime()
{
    uint8_t reg_value = readRegister(AVERAGING_AND_SAMPLE_CONFIG);

    uint8_t sampling_bits = reg_value & 0x0C; // SAMP_TIME, bits 3:2

    switch (sampling_bits)
    {
    case 0x00:
        return 320;
    case 0x04:
        return 640;
    case 0x08:
        return 1280;
    case 0x0C:
        return 2560;
    default:
        return 1280; // Default value
    }
}

/* AVERAGING
    Sets AVG (bits 6:4) of the Averaging and Sampling Configuration
    register (0x24): samples averaged per measurement. Valid values: 1, 2,
    4, 8, 16, 32, 64, 128. Others set 8.
*/
void CAP1203::setAveragingNumberOfSamples(uint8_t num_samples)
{
    uint8_t org = readRegister(AVERAGING_AND_SAMPLE_CONFIG);
    uint8_t value;
    if (num_samples == 1)
    {
        value = 0x00;
    }
    else if (num_samples == 2)
    {
        value = 0x10;
    }
    else if (num_samples == 4)
    {
        value = 0x20;
    }
    else if (num_samples == 8)
    {
        value = 0x30;
    }
    else if (num_samples == 16)
    {
        value = 0x40;
    }
    else if (num_samples == 32)
    {
        value = 0x50;
    }
    else if (num_samples == 64)
    {
        value = 0x60;
    }
    else if (num_samples == 128)
    {
        value = 0x70;
    }
    else
    {
        value = 0x30; // default: 8
    }
    writeRegister(AVERAGING_AND_SAMPLE_CONFIG, (org & 0x8F) | value);
}

/* AVERAGING
    Returns AVG of the Averaging and Sampling Configuration register
    (0x24): samples averaged per measurement.
*/
uint8_t CAP1203::getAveragingNumberOfSamples()
{
    uint8_t reg_value = readRegister(AVERAGING_AND_SAMPLE_CONFIG);

    uint8_t num_samples_bits = reg_value & 0x70; // AVG, bits 6:4

    switch (num_samples_bits)
    {
    case 0x00:
        return 1;
    case 0x10:
        return 2;
    case 0x20:
        return 4;
    case 0x30:
        return 8;
    case 0x40:
        return 16;
    case 0x50:
        return 32;
    case 0x60:
        return 64;
    case 0x70:
        return 128;
    default:
        return 8; // Default value
    }
}
