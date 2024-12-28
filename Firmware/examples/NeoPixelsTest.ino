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
  // Turn on only red color
  for (int j = 0; j < NUMPIXELS; j++) {                       // For each pixel...
    pixels.setPixelColor(j, pixels.Color(MAX_BRIGHTNESS, 0, 0));  // Set pixel's color (in RAM)
    pixels.show();                                            // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
  delay(5 * DELAYVAL);  // Pause before next pass through loop
  pixels.clear();       // Set all pixel colors to 'off'

  // Turn on only green color
  for (int j = 0; j < NUMPIXELS; j++) {                       // For each pixel...
    pixels.setPixelColor(j, pixels.Color(0, MAX_BRIGHTNESS, 0));  // Set pixel's color (in RAM)
    pixels.show();                                            // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
  delay(5 * DELAYVAL);  // Pause before next pass through loop
  pixels.clear();       // Set all pixel colors to 'off'

  // Turn on only blue color
  for (int j = 0; j < NUMPIXELS; j++) {                       // For each pixel...
    pixels.setPixelColor(j, pixels.Color(0, 0, MAX_BRIGHTNESS));  // Set pixel's color (in RAM)
    pixels.show();                                            // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
  delay(5 * DELAYVAL);  // Pause before next pass through loop
  pixels.clear();       // Set all pixel colors to 'off'
}
