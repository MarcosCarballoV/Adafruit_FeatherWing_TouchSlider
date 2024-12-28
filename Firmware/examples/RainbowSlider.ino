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
  Slider.getSliderTouched(PadsTouched, sizeof(PadsTouched));  // Get the state of the pads

  for (uint8_t i = 0; i < NUMPIXELS; i++) {                              // For each pixel
    if (PadsTouched[i]) {                                                // If the pad is touched
      uint32_t color = pixels.ColorHSV(map(i, 0, NUMPIXELS, 0, 65535));  // Calculate the color based on the position of the pad
      pixels.setPixelColor(i, color);                                    // Set the pixel color
      pixels.setBrightness(MAX_BRIGHTNESS);                              // Set the brightness
    } else {                                                             // If the pad is not touched
      pixels.setPixelColor(i, 0);                                        // Turn off LED if not touched
    }
  }
  pixels.show();  // Show the pixels
}