/*
 * Marcos Abraham Carballo Vazquez
 * Original Creation Date: Dicember 5, 2024
 * https://github.com/MarcosCarballoV/Adafruit_FeatherWing_TouchSlider
 * */

#ifndef TOUCHSLIDER_H
#define TOUCHSLIDER_H

/*********************** EXTERNAL LIBRARIES **********************/

#include <Arduino.h>
#include <Ticker.h>

#include "CAP1208.h"
#include "Logger.h"

/*********************** LIBRARY OPTIONS **********************/
#define START_WITH_SWIPE_FINE     // Enable swipe fine by default, comment this line to disable
#define START_PRINT_SWIPE_STATUS  // Print the swipe status by default, comment this line to disable
// #define START_PRINT_SLIDER_TOUCHED            // Print the slider touched by default, comment this line to disable
#define TOUCH_PAD_CAP1208 8

/*********************** LIBRARY OPTIONS **********************/

class TouchSlider {
 public:
  TouchSlider(CAP1208* sensor);
  void start();
  void stop();
  void resume();

  int8_t getSwipeStatus();
  int8_t getSwipeStatusFine();
  void getSliderTouched(bool sliderTouched[], uint8_t numSliderPins);  // Get the SliderTouched

  //  Enable/Disable functions
  void enableSwipeFine() { _enableSwipeFine = true; };    // Enable swipe fine
  void disableSwipeFine() { _enableSwipeFine = false; };  // Disable swipe fine

  // Enable/Disable print functions
  void enablePrintSliderTouched() { _enablePrintSliderTouched = true; };    // Enable print array of pads on slider which were touched
  void disablePrintSliderTouched() { _enablePrintSliderTouched = false; };  // Disable print array of pads on slider which were touched
  void enablePrintSwipeStatus() { _enablePrintSwipeStatus = true; };        // Enable print swipe status (include swipe fine status)
  void disablePrintSwipeStatus() { _enablePrintSwipeStatus = false; };      // Disable print swipe status (include swipe fine status)

 private:
  CAP1208* CAP1208_Sensor;
  bool _sliderRunning = false;

  const uint16_t UPDATE_INTERVAL = 50;
  enum { NO_CHANGE,
         SWIPE_UP,
         SWIPE_DOWN };

  Ticker sliderTicker;
  // Static configuration and runtime state
  int16_t _lastValue, _actualValue;
  uint8_t _sliderState = NO_CHANGE;
  uint8_t _numSliderPins = TOUCH_PAD_CAP1208;

  bool _SliderTouched[TOUCH_PAD_MAX];
  static int16_t _sliderValue[TOUCH_PAD_MAX];

  int8_t _swipeCount = 0;
  int8_t _swipeUpCount = 0;
  int8_t _swipeDownCount = 0;

  int8_t _swipeFineUpCount = 0;
  int8_t _swipeFineDownCount = 0;

  bool firstTouch = true;
  bool firstPadTop = false;
  bool firstPadBot = false;

  bool _enablePrintSwipeStatus = false;    // Indicates whether to print the swipe status
  bool _enablePrintSliderTouched = false;  // Indicates whether to print the Slider Touched
  bool _enableSwipeFine = false;           // Indicates whether to enable Swipe Fine
  bool _enableTouchButtons = false;        // Indicates whether to enable Touch Buttons

  void begin();
  void setDefaultConfiguration();
  static void update(TouchSlider* self);
  static void printAllPadTouched();
  void printSliderTouched();
  void analyzeGesture(uint8_t numSliders);
  void printSliderValues(uint8_t numSliders);

  static void checkSliderStatus(TouchSlider* self, bool& padTouchedFound, int8_t& firstTouchedIndex,
                                int8_t& lastTouchedIndex, uint8_t& touchedPadCount);
  static void handleNoTouch(TouchSlider* self);
  static void handleTouch(TouchSlider* self, int8_t firstTouchedIndex, int8_t lastTouchedIndex, uint8_t touchedPadCount);

  void resetFirstTouches();
};
#endif