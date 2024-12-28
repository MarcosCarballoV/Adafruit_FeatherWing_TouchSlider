#include <Adafruit_NeoPixel.h>  // NeoPixel library
#include <Arduino.h>            // Arduino library
#include <Wire.h>               // I2C library
#include "CAP1208.h"      // Capacitive sensor library
#include "Logger.h"       // Logger library
#include "TouchSlider.h"  // Touch slider library

// Pins designed for NeoPixels, edit according to your setup
#define PIN 4              // Pin connected to NeoPixels
#define NUMPIXELS 8        // NeoPixel ring size
#define DELAYVAL 80        // Time (in milliseconds) to pause between pixels
#define MAX_BRIGHTNESS 50  // The maximum brightness of the LED
#define STEP_BRIGHTNESS 5  // The step brightness of the LED
#define STEP_PERCENTAGE 5  // The step percentage of the LED

// Array to store the state of the pads
bool PadsTouched[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Objects
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // NeoPixel object
CAP1208 CAP1208_Sensor;                                          // CAP1208 object
TouchSlider Slider(&CAP1208_Sensor);                             // TouchSlider object

void setup() {
  Wire.begin();          // Join I2C bus
  Serial.begin(115200);  // Start serial for output

  log_i("Starting up with NeoPixel strip...");
  delay(100);
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();  // Set all pixel colors to 'off'

  log_i("Starting up with CAP1208 sensor...");
  CAP1208_Sensor.begin();                          // Initialize the CAP1208 sensor
  CAP1208_Sensor.ConfigureMultiTouch(4);           // Configure MultiTouch to 4 pads
  CAP1208_Sensor.setSensitivity(SENSITIVITY_32X);  // Set sensitivity to 32x on startup (change this variable to change sensitivity according to your needs)

  Slider.start();  // Start the touch slider
}

void loop() {
  static uint8_t percentage = 0;                                      // Percentage variable
  static uint32_t currentColor = pixels.Color(0, MAX_BRIGHTNESS, 0);  // Color variable

  int8_t swipeStatus = Slider.getSwipeStatus();  // Get swipe status

  if (swipeStatus != 0) {                                  // If swipe status is not 0
    if (swipeStatus < 0) {                                 // If swipe status is less than 0
      if (percentage != 0) {                               // If percentage is not 0
        percentage -= STEP_PERCENTAGE * abs(swipeStatus);  // Decrease percentage
      }
    } else {                                               // If swipe status is greater than 0
      if (percentage != 100) {                             // If percentage is not 100
        percentage += STEP_PERCENTAGE * abs(swipeStatus);  // Increase percentage
      }
    }
  }

  uint8_t ledsOn = map(percentage, 0, 100, 0, NUMPIXELS);           // Map percentage to number of LEDs
  uint8_t brightness = map(percentage, 0, 100, 0, MAX_BRIGHTNESS);  // Map percentage to brightness (This is optional, you can use MAX_BRIGHTNESS directly)

  currentColor = pixels.Color(0, brightness, 0);  // Set color according to percentage
  if (ledsOn != 0) {                              // If number of LEDs is not 0
    pixels.clear();                               // Clear all pixels
    for (uint8_t i = 0; i < ledsOn; i++) {        // For each LED
      pixels.setPixelColor(i, currentColor);      // Set pixel color
    }
  } else
    pixels.clear();  // If number of LEDs is 0, clear all pixels
  pixels.show();     // Show pixels
}