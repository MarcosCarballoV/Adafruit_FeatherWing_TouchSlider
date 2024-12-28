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
#define MAX_BRIGHTNESS 50  // The maximum brightness of the LED (In this example we recommend multiple of STEP_BRIGHTNESS)
#define STEP_BRIGHTNESS 5  // The step brightness of the LED

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
  static bool isFirstConfig = true;              // First configuration
  static uint8_t counter = 0;                    // Counter
  static uint32_t color;                         // Color
  int8_t swipeStatus = Slider.getSwipeStatus();  // Get the swipe status

  if (isFirstConfig) {                           // If it is the first configuration (only once)
    isFirstConfig = false;                       // Set isFirstConfig to false
    color = pixels.Color(0, MAX_BRIGHTNESS, 0);  // Set the color (green)
    for (uint8_t i = 0; i < NUMPIXELS; i++) {    // For each pixel
      pixels.setPixelColor(i, color);            // Set the pixel color
    }
    pixels.show();  // Send the updated pixel colors to the hardware.
  }

  if (swipeStatus != 0) {                               // If the swipe status is not 0
    if (swipeStatus > 0) {                              // If the swipe status is positive
      if (counter != 0) {                               // If the counter is not 0
        counter -= STEP_BRIGHTNESS * abs(swipeStatus);  // Decrease the counter (Add a little green to the color)
      }
    } else {                                            // If the swipe status is negative
      if (counter != MAX_BRIGHTNESS) {                  // If the counter is not MAX_BRIGHTNESS
        counter += STEP_BRIGHTNESS * abs(swipeStatus);  // Increase the counter (Add a little red to the color)
      }
    }
    color = pixels.Color(counter, MAX_BRIGHTNESS - counter, 0);  // Set the color according to the counter
    for (uint8_t i = 0; i < NUMPIXELS; i++) {                    // For each pixel
      pixels.setPixelColor(i, color);                            // Set the pixel color
    }
    pixels.show();  // Send the updated pixel colors to the hardware.
  }

  int8_t _SaveStatusFine = Slider.getSwipeStatusFine();  // Get the swipe status fine

  if (_SaveStatusFine != 0) {    // If the swipe status fine is not 0
    if (_SaveStatusFine > 0) {   // If the swipe status fine is positive
      counter = MAX_BRIGHTNESS;  // Set the counter to MAX_BRIGHTNESS (Return the color to RED)
    } else {                     // If the swipe status fine is negative
      counter = 0;               // Set the counter to 0 (Return the color to GREEN)
    }

    color = pixels.Color(counter, MAX_BRIGHTNESS - counter, 0);  // Set the color according to the counter
    for (uint8_t i = 0; i < NUMPIXELS; i++) {                    // For each pixel
      pixels.setPixelColor(i, color);                            // Set the pixel color
    }
    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}