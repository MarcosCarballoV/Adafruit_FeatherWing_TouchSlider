/*
 * Marcos Abraham Carballo Vazquez
 * Original Creation Date: Dicember 5, 2024
 * https://github.com/MarcosCarballoV/Adafruit_FeatherWing_TouchSlider
 * */

#ifndef CAP1208_H
#define CAP1208_H

#include <Arduino.h>
#include <Wire.h>

#include "CAP1208_Registers.h"

// Capacitive sensor input (pg. 23)
#define OFF 0x00  // No touch detecetd
#define ON 0x01   // Check capacitive sensor input (pg. 23)

// Power button hold time to generate interrupt (pg. 44)
#define PWR_TIME_280_MS 0x00   // 280 ms
#define PWR_TIME_560_MS 0x01   // 560 ms
#define PWR_TIME_1120_MS 0x02  // 1.12 sec
#define PWR_TIME_2240_MS 0x03  // 2.24 sec

// Sensitivity for touch detection (pg. 25)
#define SENSITIVITY_128X 0x00  // Most sensitive
#define SENSITIVITY_64X 0x01
#define SENSITIVITY_32X 0x02
#define SENSITIVITY_16X 0x03
#define SENSITIVITY_8X 0x04
#define SENSITIVITY_4X 0x05
#define SENSITIVITY_2X 0x06
#define SENSITIVITY_1X 0x07  // Least sensitive

// Sensitivity Control Reg (pg. 25)
typedef union {
  struct
  {
    uint8_t BASE_SHIFT : 4;
    uint8_t DELTA_SENSE : 3;
    uint8_t EMPTY_1 : 1;
  } SENSITIVITY_CONTROL_FIELDS;
  uint8_t SENSITIVITY_CONTROL_COMBINED;
} SENSITIVITY_CONTROL_REG;

// General Status Reg (pg. 23)
typedef union {
  struct
  {
    uint8_t TOUCH : 1;
    uint8_t MTP : 1;
    uint8_t MULT : 1;
    uint8_t EMPTY_1 : 1;
    uint8_t PWR : 1;
    uint8_t ACAL_FAIL : 1;
    uint8_t BC_OUT : 1;
    uint8_t EMPTY_2 : 1;
  } GENERAL_STATUS_FIELDS;
  uint8_t GENERAL_STATUS_COMBINED;
} GENERAL_STATUS_REG;

// Sensor Input Status Reg (pg. 23)
typedef union {
  struct
  {
    uint8_t CS1 : 1;
    uint8_t CS2 : 1;
    uint8_t CS3 : 1;
    uint8_t CS4 : 1;
    uint8_t CS5 : 1;
    uint8_t CS6 : 1;
    uint8_t CS7 : 1;
    uint8_t CS8 : 1;
  } SENSOR_INPUT_STATUS_FIELDS;
  uint8_t SENSOR_INPUT_STATUS_COMBINED;
} SENSOR_INPUT_STATUS_REG;

// Main Control Register (pg. 22)
typedef union {
  struct
  {
    uint8_t INT : 1;
    uint8_t EMPTY_1 : 3;
    uint8_t DSLEEP : 1;
    uint8_t STBY : 1;
    uint8_t EMPTY_2 : 2;
  } MAIN_CONTROL_FIELDS;
  uint8_t MAIN_CONTROL_COMBINED;
} MAIN_CONTROL_REG;

// Power Button Register (pg. 43)
typedef union {
  struct
  {
    uint8_t PWR_BTN : 3;
    uint8_t EMPTY_1 : 5;
  } POWER_BUTTON_FIELDS;
  uint8_t POWER_BUTTON_COMBINED;
} POWER_BUTTON_REG;

// Power Button Configuration Register (pg. 43)
typedef union {
  struct
  {
    uint8_t PWR_TIME : 2;
    uint8_t PWR_EN : 1;
    uint8_t EMPTY_1 : 1;
    uint8_t STBY_PWR_TIME : 2;
    uint8_t STBY_PWR_EN : 1;
    uint8_t EMPTY_2 : 1;
  } POWER_BUTTON_CONFIG_FIELDS;
  uint8_t POWER_BUTTON_CONFIG_COMBINED;
} POWER_BUTTON_CONFIG_REG;

// Interrupt Enable Register (pg. 33)
typedef union {
  struct
  {
    uint8_t CS1_INT_EN : 1;
    uint8_t CS2_INT_EN : 1;
    uint8_t CS3_INT_EN : 1;
    uint8_t CS4_INT_EN : 1;
    uint8_t CS5_INT_EN : 1;
    uint8_t CS6_INT_EN : 1;
    uint8_t CS7_INT_EN : 1;
    uint8_t CS8_INT_EN : 1;
  } INTERRUPT_ENABLE_FIELDS;
  uint8_t INTERRUPT_ENABLE_COMBINED;
} INTERRUPT_ENABLE_REG;

typedef union {
  struct
  {
    uint8_t EMPTY_1 : 2;
    uint8_t B_MULT_T : 2;
    u_int8_t EMPTY_2 : 4;
    uint8_t MULT_BLK_EN : 1;
  } MULTI_TOUCH_FIELDS;
  uint8_t MULTI_TOUCH_COMBINED;
} MULTI_TOUCH_REG;

typedef union {
  struct
  {
    uint8_t MTP_ALERT : 1;
    uint8_t COMP_PTRN : 1;
    uint8_t MTP_THRESHOLD : 2;
    uint8_t EMPTY_1 : 3;
    uint8_t MTP_EN : 1;
  } PATTERN_FIELDS;
  uint8_t PATTERN_COMBINED;
} MULTI_TOUCH_PATTERN_REG;

////////////////////////////////
// CAP1208 Class Declearation //
////////////////////////////////
class CAP1208 {
 public:
  // Constructor
  CAP1208(byte addr = CAP1208ADDR);

  bool begin(TwoWire &wirePort = Wire, uint8_t deviceAddress = CAP1208ADDR);
  bool start();
  bool isConnected();
  void setSensitivity(uint8_t sensitivity);
  uint8_t getSensitivity();

  // Configurations
  void StandbyMode();
  void ActiveMode();
  void SleepMode();
  void ConfigureMultiTouch(uint8_t number);

  // Gett the Touch Data
  void getTouchData(bool data[8]);

  bool isTouched();

  // Clears INT bit
  void clearInterrupt();

  // Configures if interrupt triggers on touch
  void setInterruptDisabled();
  void setInterruptEnabled();
  bool isInterruptEnabled();

  void checkMainControl();
  void checkStatus();

  uint8_t readID();

 private:
  TwoWire *_i2cPort = NULL;  // The generic connection to user's chosen I2C hardware
  uint8_t _deviceAddress;    // Keeps track of I2C address. setI2CAddress changes this.

  // Read and write to registers
  byte readRegister(CAP1208_Register reg);
  void readRegisters(CAP1208_Register reg, byte *buffer, byte len);
  void writeRegister(CAP1208_Register reg, byte data);
  void writeRegisters(CAP1208_Register reg, byte *buffer, byte len);
};

#endif