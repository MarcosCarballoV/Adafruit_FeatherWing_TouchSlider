#include "CAP1208.h"

/**
 * @brief Constructor for the TouchSlider class.
 *
 * @param addr The address of the CAP1208 sensor.
 */
CAP1208::CAP1208(byte addr) {
  _deviceAddress = addr;
}

/**
 * @brief Initialize the CAP1208 sensor.
 *
 * @param wirePort The Wire port to use for communication.
 * @param deviceAddress The address of the CAP1208 sensor.
 *
 * @return true if the sensor is initialized successfully.
 */
bool CAP1208::begin(TwoWire &wirePort, uint8_t deviceAddress) {
  // Set device address and wire port to private variable
  _deviceAddress = deviceAddress;
  _i2cPort = &wirePort;

  if (isConnected() == false) {
    log_e("CAP1208 not connected");
    return false;
  }

  // Read PROD_ID register
  byte prodIDValue = readRegister(PRODUCT_ID);
  // PROD_ID should always be 0x6D
  if (prodIDValue != PROD_ID_VALUE) {
    log_e("Product ID not correct, 0x%x", prodIDValue);
    return false;
  }

  return true;  // Success
}

/**
 *  @brief Check if the sensor is connected
 *
 *  @return true if the sensor is connected
 */
bool CAP1208::isConnected() {
  for (byte i = 0; i < 5; i++) {
    /* After inspecting with logic analyzer, the device fails
        to connect for unknown reasons. The device typically connects
        after two calls. We included a for loop to allow for
        multiple calls to the device.
    */
    _i2cPort->beginTransmission((uint8_t)_deviceAddress);
    if (_i2cPort->endTransmission() == 0)
      return (true);  // Sensor did not ACK
  }

  return (false);
}

/**
 * @brief Checks the main control register.
 * @note See datasheet on Main Control Register
 */
void CAP1208::checkMainControl() {
  MAIN_CONTROL_REG reg;
  reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CTRL_REG);
}

/**
 * @brief Checks the general status register
 * @note See datasheet on General Status Register
 */
void CAP1208::checkStatus() {
  GENERAL_STATUS_REG reg;
  reg.GENERAL_STATUS_COMBINED = readRegister(GEN_STATUS);
}

/**
 * @brief Clears the interrupt pin
 */
void CAP1208::clearInterrupt() {
  MAIN_CONTROL_REG reg;
  reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CTRL_REG);
  reg.MAIN_CONTROL_FIELDS.INT = 0x00;
  writeRegister(MAIN_CTRL_REG, reg.MAIN_CONTROL_COMBINED);
}

/**
 * @brief Disables all interrupts
 */
void CAP1208::setInterruptDisabled() {
  INTERRUPT_ENABLE_REG reg;
  reg.INTERRUPT_ENABLE_COMBINED = readRegister(INT_ENABLE);
  reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS4_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS5_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS6_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS7_INT_EN = 0x00;
  reg.INTERRUPT_ENABLE_FIELDS.CS8_INT_EN = 0x00;
  writeRegister(INT_ENABLE, reg.INTERRUPT_ENABLE_COMBINED);
}

/**
 * @brief Sets all interrupts
 */
void CAP1208::setInterruptEnabled() {
  INTERRUPT_ENABLE_REG reg;
  reg.INTERRUPT_ENABLE_COMBINED = readRegister(INT_ENABLE);
  reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS4_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS5_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS6_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS7_INT_EN = 0x01;
  reg.INTERRUPT_ENABLE_FIELDS.CS8_INT_EN = 0x01;
  writeRegister(INT_ENABLE, reg.INTERRUPT_ENABLE_COMBINED);
}

/**
 * @brief Checks if interrupts are enabled
 *
 * @return true if interrupts are enabled
 */
bool CAP1208::isInterruptEnabled() {
  INTERRUPT_ENABLE_REG reg;
  reg.INTERRUPT_ENABLE_COMBINED = readRegister(INT_ENABLE);
  if (reg.INTERRUPT_ENABLE_FIELDS.CS1_INT_EN == 0x01 && reg.INTERRUPT_ENABLE_FIELDS.CS2_INT_EN == 0x01 && reg.INTERRUPT_ENABLE_FIELDS.CS3_INT_EN == 0x01) {
    return true;
  }
  return false;
}

/**
 * @brief Sets the sensitivity
 * 
 * @param  sensitivity: Sensitivity value, SENSITIVITY_128X to SENSITIVITY_1X (Most sensitive to least sensitive)
 * @note See datasheet on Sensitivity, default is SENSITIVITY_32X
 */
void CAP1208::setSensitivity(uint8_t sensitivity) {
  SENSITIVITY_CONTROL_REG reg;
  reg.SENSITIVITY_CONTROL_COMBINED = readRegister(SENSITIVITY);
  if (sensitivity == SENSITIVITY_128X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_128X;
  } else if (sensitivity == SENSITIVITY_64X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_64X;
  } else if (sensitivity == SENSITIVITY_32X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_32X;
  } else if (sensitivity == SENSITIVITY_16X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_16X;
  } else if (sensitivity == SENSITIVITY_8X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_8X;
  } else if (sensitivity == SENSITIVITY_4X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_4X;
  } else if (sensitivity == SENSITIVITY_1X) {
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_1X;
  } else {
    // Default case: calibrated for CAP1208 touch sensor
    reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE = SENSITIVITY_2X;
  }
  writeRegister(SENSITIVITY, reg.SENSITIVITY_CONTROL_COMBINED);
}

/**
 * @brief Gets the sensitivity
 * 
 * @return Sensitivity value, SENSITIVITY_128X to SENSITIVITY_1X (Most sensitive to least sensitive)
 */
uint8_t CAP1208::getSensitivity() {
  SENSITIVITY_CONTROL_REG reg;
  reg.SENSITIVITY_CONTROL_COMBINED = readRegister(SENSITIVITY);
  uint16_t sensitivity = reg.SENSITIVITY_CONTROL_FIELDS.DELTA_SENSE;
  if (sensitivity == SENSITIVITY_128X) {
    return 128;
  } else if (sensitivity == SENSITIVITY_64X) {
    return 64;
  } else if (sensitivity == SENSITIVITY_32X) {
    return 32;
  } else if (sensitivity == SENSITIVITY_16X) {
    return 16;
  } else if (sensitivity == SENSITIVITY_8X) {
    return 8;
  } else if (sensitivity == SENSITIVITY_4X) {
    return 4;
  } else if (sensitivity == SENSITIVITY_2X) {
    return 2;
  } else if (sensitivity == SENSITIVITY_1X) {
    return 1;
  } else {
    // Error - no possible register value (pg. 25)
    return 0;
  }
}

/**
 * @brief Checks if the sensor is touched
 * @note See datasheet on General Status Register
 * @retval true if the sensor is touched
 */
bool CAP1208::isTouched() {
  GENERAL_STATUS_REG reg;
  reg.GENERAL_STATUS_COMBINED = readRegister(GEN_STATUS);

  // Touch detected
  if (reg.GENERAL_STATUS_FIELDS.TOUCH == ON) {
    clearInterrupt();
    return true;
  }
  return false;
}

/**
 * @brief Reads the product ID
 * @note  See datasheet on Product ID
 * @retval The product ID
 */
uint8_t CAP1208::readID() {
  uint8_t id;

  readRegisters(PRODUCT_ID, &id, 1);

  return id;
}

/**
 * @brief Sets the sensor to standby mode
 */
void CAP1208::StandbyMode() {
  MAIN_CONTROL_REG reg;
  reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CTRL_REG);
  reg.MAIN_CONTROL_FIELDS.STBY = true;
  writeRegister(MAIN_CTRL_REG, reg.MAIN_CONTROL_COMBINED);
}

/**
 * @brief Sets the sensor to active mode
 */
void CAP1208::ActiveMode() {
  MAIN_CONTROL_REG reg;
  reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CTRL_REG);
  reg.MAIN_CONTROL_FIELDS.STBY = false;
  writeRegister(MAIN_CTRL_REG, reg.MAIN_CONTROL_COMBINED);
}

/**
 * @brief Sets the sensor to sleep mode
 */
void CAP1208::SleepMode() {
  MAIN_CONTROL_REG reg;
  reg.MAIN_CONTROL_COMBINED = readRegister(MAIN_CTRL_REG);
  reg.MAIN_CONTROL_FIELDS.DSLEEP = true;
  writeRegister(MAIN_CTRL_REG, reg.MAIN_CONTROL_COMBINED);
}

/**
 * @brief Set the multi touch mode, this option enables multiple touch detection allowing for a maximum of 4 touches
 * @param  number: Number of touches, 1 (Single touch) to 4 (Maximum of 4 touches)
 */
void CAP1208::ConfigureMultiTouch(u_int8_t number) {
  MULTI_TOUCH_REG reg;
  if (number != 0) {
    number -= 1;
    if (number >= 4) {
      number = 3;
    }
    reg.MULTI_TOUCH_COMBINED = readRegister(MULTITOUCH);
    reg.MULTI_TOUCH_FIELDS.MULT_BLK_EN = true;
    reg.MULTI_TOUCH_FIELDS.B_MULT_T = number;
    writeRegister(MULTITOUCH, reg.MULTI_TOUCH_COMBINED);
  } else {
    reg.MULTI_TOUCH_COMBINED = readRegister(MULTITOUCH);
    reg.MULTI_TOUCH_FIELDS.MULT_BLK_EN = false;
    reg.MULTI_TOUCH_FIELDS.B_MULT_T = false;
    writeRegister(MULTITOUCH, reg.MULTI_TOUCH_COMBINED);
  }
}

/**
 * @brief Reads the touch data
 *
 * @param data: Array to store the touch data
 */
void CAP1208::getTouchData(bool data[8]) {
  SENSOR_INPUT_STATUS_REG reg;
  reg.SENSOR_INPUT_STATUS_COMBINED = readRegister(SENSOR_INPUTS);

  data[0] = reg.SENSOR_INPUT_STATUS_FIELDS.CS1;
  data[1] = reg.SENSOR_INPUT_STATUS_FIELDS.CS2;
  data[2] = reg.SENSOR_INPUT_STATUS_FIELDS.CS3;
  data[3] = reg.SENSOR_INPUT_STATUS_FIELDS.CS4;
  data[4] = reg.SENSOR_INPUT_STATUS_FIELDS.CS5;
  data[5] = reg.SENSOR_INPUT_STATUS_FIELDS.CS6;
  data[6] = reg.SENSOR_INPUT_STATUS_FIELDS.CS7;
  data[7] = reg.SENSOR_INPUT_STATUS_FIELDS.CS8;

  clearInterrupt();
}

/**
 * @brief  Reads a single register
 * 
 * @param  reg: Register to read
 * @retval Value of the register
 */
byte CAP1208::readRegister(CAP1208_Register reg) {
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->endTransmission(false);                // endTransmission but keep the connection active
  _i2cPort->requestFrom(_deviceAddress, (byte)1);  // Ask for 1 byte, once done, bus is released by default

  // Wait for the data to come back
  if (_i2cPort->available()) {
    return _i2cPort->read();  // Return this one byte
  } else {
    return 0;
  }
}

/**
 * @brief  Reads multiple registers
 *  
 * @param  reg: Register to read
 * @param  *buffer: Array to store the data
 * @param  len: Number of bytes to read
 */
void CAP1208::readRegisters(CAP1208_Register reg, byte *buffer, byte len) {
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->endTransmission(false);            // endTransmission but keep the connection active
  _i2cPort->requestFrom(_deviceAddress, len);  // Ask for bytes, once done, bus is released by default

  // Wait for data to come back
  if (_i2cPort->available() == len) {
    // Iterate through data from buffer
    for (int i = 0; i < len; i++)
      buffer[i] = _i2cPort->read();
  }
}

/**
 * @brief  Writes a single register
 * 
 * @param  reg: Register to write
 * @param  data: Data to write
 */
void CAP1208::writeRegister(CAP1208_Register reg, byte data) {
  writeRegisters(reg, &data, 1);
}

/**
 * @brief  Writes a multiple registers
 * 
 * @param  reg: Register to write
 * @param  *buffer: Data to write
 * @param  len: Number of bytes to write
 */
void CAP1208::writeRegisters(CAP1208_Register reg, byte *buffer, byte len) {
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  for (int i = 0; i < len; i++)
    _i2cPort->write(buffer[i]);
  _i2cPort->endTransmission();  // Stop transmitting
}