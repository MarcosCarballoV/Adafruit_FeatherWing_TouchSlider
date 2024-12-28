/*
* Marcos Abraham Carballo Vazquez
* Original Creation Date: Dicember 5, 2024
* https://github.com/MarcosCarballoV/Adafruit_FeatherWing_TouchSlider
* */

#ifndef CAP1208_REGISTERS_H
#define CAP1208_REGISTERS_H

#define CAP1208ADDR	  0x28		    // 0x28 - not shifted

#define PROD_ID_VALUE 0x6B        // Product ID - always the same (pg. 22)

enum CAP1208_Register
{
  // Main Controller Registers
  MAIN_CTRL_REG =		  0x00,
  GEN_STATUS	=			  0x02,
  SENSOR_INPUTS =			0x03,
  NOISE_FLAG	=			  0x0A,
  SENS1DELTACOUNT =		0x10,
  SENS2DELTACOUNT =		0x11,
  SENS3DELTACOUNT =		0x12,
  SENS4DELTACOUNT =		0x13,
  SENS5DELTACOUNT =		0x14,
  SENS6DELTACOUNT =		0x15,
  SENS7DELTACOUNT =		0x16,
  SENS8DELTACOUNT =		0x17,
  SENSITIVITY =				0x1F,
  CONFIG1 =					  0x20,
  SENSINPUTEN =				0x21,
  SENSINCONF1 =				0x22,
  SENSINCONF2 =				0x23,
  AVERAGE_SAMP_CONF =	0x24,
  CAL_ACTIV =				  0x26,
  INT_ENABLE =				0x27,
  REPEAT_RATE =				0x28,
  MULTITOUCH =				0x2A,
  MULTIPATCONF =			0x2B,
  MULTIPATTERN =			0x2D,
  BASECOUNT =				  0x2E,
  RECALCONFIG =				0x2F,
  S1THRESHOLD =				0x30,
  S2THRESHOLD =				0x31,
  S3THRESHOLD =				0x32,
  S4THRESHOLD =				0x33,
  S5THRESHOLD =				0x34,
  S6THRESHOLD =				0x35,
  S7THRESHOLD =				0x36,
  S8THRESHOLD =				0x37,
  SENSTHRESHOLD =			0x38,
  // Standby Configuration Registers
  STANDBYCHAN =				0x40,
  STANDBYCONF =				0x41,
  STANDBY_SENS =		  0x42,
  STANDBY_THRE =			0x43,
  CONFIG2 =					  0x44,
  // Base Count Registers
  S1BASECOUNT =				0x50,
  S2BASECOUNT =				0x51,
  S3BASECOUNT =				0x52,
  S4BASECOUNT =				0x53,
  S5BASECOUNT =				0x54,
  S6BASECOUNT =				0x55,
  S7BASECOUNT =				0x56,
  S8BASECOUNT =				0x57,
  // Power Button Registers
  PWR_BUTTON =				0x60,
  PWR_CONFIG =				0x61,
  // Calibration Registers
  S1INPCAL =				  0xB1,	
  S2INPCAL =				  0xB2,
  S3INPCAL =				  0xB3,
  S4INPCAL =				  0xB4,
  S5INPCAL =				  0xB5,
  S6INPCAL =				  0xB6,
  S7INPCAL =				  0xB7,
  S8INPCAL =				  0xB8,
  S1CALLSB =				  0xB9,
  S2CALLSB =				  0xBA,
  // ID Registers
  PRODUCT_ID =			  0xFD,
  MAN_ID =					  0xFE,
  REV =						    0xFF,	
};

#endif