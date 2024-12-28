#include <Adafruit_NeoPixel.h>  // NeoPixel library
#include <Arduino.h>            // Arduino library
#include <Wire.h>               // I2C library
#include "CAP1208.h"      // Capacitive sensor library
#include "Logger.h"       // Logger library
#include "TouchSlider.h"  // Touch slider library

// Pins designed for NeoPixels, edit according to your setup
#define PIN 4               // Pin connected to NeoPixels
#define NUMPIXELS 8         // NeoPixel ring size
#define DELAYVAL 80         // Time (in milliseconds) to pause between pixels
#define MAX_BRIGHTNESS 50   // The maximum brightness of the LED (In this example we recommend multiple of STEP_BRIGHTNESS)
#define STEP_BRIGHTNESS 10  // The step brightness of the LED

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
  static uint8_t _BRIGHTNESS = MAX_BRIGHTNESS;  // The brightness of the LED
  int8_t _SaveStatusFine = 0;                   // The status of the swipe, reset every loop

  Slider.getSliderTouched(PadsTouched, sizeof(PadsTouched));  // Get the state of the pads

  pixels.clear();                                      // Clear the strip
  for (uint8_t i = 0; i < sizeof(PadsTouched); i++) {  // For each pad
    if (PadsTouched[i]) {                              // If the pad is touched
      pixels.setPixelColor(i, pixels.Color(_BRIGHTNESS / 2, _BRIGHTNESS / 2, _BRIGHTNESS));  // Set pixel's color
    }
  }
  pixels.show();  // Send the updated pixel colors to the hardware

  _SaveStatusFine = Slider.getSwipeStatusFine();  // Get the status of the swipe

  if (_SaveStatusFine != 0) {  // If the status of the swipe is not 0
    log_i("The swipe status fine is %d", _SaveStatusFine);
    if (_SaveStatusFine > 0) {            // If the swipe status fine is positive
      if (_BRIGHTNESS != MAX_BRIGHTNESS)  // If the brightness is not MAX_BRIGHTNESS
        _BRIGHTNESS += STEP_BRIGHTNESS;   // Add 10 to the brightness
    } else {
      if (_BRIGHTNESS != 0)              // If the brightness is not 0
        _BRIGHTNESS -= STEP_BRIGHTNESS;  // Subtract 10 from the brightness
    }

    if (_BRIGHTNESS > MAX_BRIGHTNESS) {  // If the brightness is greater than MAX_BRIGHTNESS
      _BRIGHTNESS = MAX_BRIGHTNESS;      // Set the brightness to MAX_BRIGHTNESS
    } else if (_BRIGHTNESS < 0) {        // If the brightness is less than 0
      _BRIGHTNESS = 0;                   // Set the brightness to 0
    }
    log_i("The brightness is %d", _BRIGHTNESS);
  }
}
