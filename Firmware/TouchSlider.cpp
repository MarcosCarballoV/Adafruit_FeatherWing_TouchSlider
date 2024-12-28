#include "TouchSlider.h"

int16_t TouchSlider::_sliderValue[TOUCH_PAD_MAX];

/**
 * @brief Constructor for the TouchSlider class.
 * 
 * @param sensor Pointer to a CAP1208 sensor object.
 * This constructor initializes the TouchSlider instance with a given CAP1208 sensor.
 */
TouchSlider::TouchSlider(CAP1208* sensor) {
  CAP1208_Sensor = sensor;  // Store the pointer to the CAP1208 sensor
}


/**
 * @brief Start the touch slider.
 *
 * This function initializes the touch slider by setting a default configuration, calling the begin method, and starting the slider update process.
 */
void TouchSlider::start() {
  setDefaultConfiguration();
  begin();
}


/**
 * @brief Initialize the touch slider.
 *
 * This function initializes the touch slider by configuring the touch pads, setting up a software filter for capacitance change detection,
 * calibrating thresholds, and starting the slider update process.
 */
void TouchSlider::begin() {
  log_i("Initializing touch slider...");
  _sliderRunning = true;  // Mark that the slider is running

  sliderTicker.attach_ms(UPDATE_INTERVAL, update, this);    // Attach a timer interrupt to periodicall
  // Attach a timer interrupt to periodically update the slider
  log_i("Touch slider initialized!");
}

/**
 * @brief Stop the touch slider timer.
 *
 * This function stops the timer responsible for updating the touch slider. It detaches the timer, marking that the slider is not currently running.
 */
void TouchSlider::stop() {
  if (_sliderRunning) {
    sliderTicker.detach();  // Stop the timer if it is running
    _sliderRunning = false;  // Mark that the timer is not running
  }
}

/**
 * @brief Resume the touch slider timer.
 *
 * This function resumes the timer for updating the touch slider. If the timer is not currently running, it attaches the timer and marks that the slider is in operation.
 */
void TouchSlider::resume() {
  if (!_sliderRunning) {
    sliderTicker.attach_ms(UPDATE_INTERVAL, update, this);  // Restart the timer if it is not running
    _sliderRunning = true;  // Mark that the timer is running
  }
}


/**
 * @brief  Set the default configuration for the TouchSlider.
 */
void TouchSlider::setDefaultConfiguration()
{
  #ifdef START_PRINT_SWIPE_STATUS
    enablePrintSwipeStatus();         // Enable print swipe status
  #endif
  #ifdef START_PRINT_SLIDER_TOUCHED
    enablePrintSliderTouched();       // Enable print slider touched
  #endif
  #ifdef START_WITH_SWIPE_FINE
    enableSwipeFine();                // Enable swipe fine
  #endif
}


/**
 * @brief Get the swipe status of the TouchSlider.
 *
 * This function calculates and returns the swipe status, which is the difference between the counts of swipe-down and swipe-up gestures.
 * It resets the swipe counts after retrieving the swipe status.
 *
 * @return An int8_t value representing the swipe status.
 *   - Positive values indicate swipe-down gestures.
 *   - Negative values indicate swipe-up gestures.
 *   - 0 indicates no swipe.
 */
int8_t TouchSlider::getSwipeStatus() {
  int8_t swipeStatus = _swipeDownCount - _swipeUpCount;   // Calculate the swipe status as the difference between swipe-down and swipe-up counts

  // Reset the swipe counts after retrieving the swipe status
  _swipeUpCount = 0;
  _swipeDownCount = 0;
  return swipeStatus;
}

/**
 * @brief Get the swipe status fine of the TouchSlider.
 *
 * This function calculates and returns the swipe status fine, which is the difference between the counts of swipe-down and swipe-up gestures.
 * It resets the swipe counts after retrieving the swipe status.
 *
 * @return An int8_t value representing the swipe status fine.
 *   - Positive values indicate swipe-down gestures.
 *   - Negative values indicate swipe-up gestures.
 *   - 0 indicates no swipe.
 */
int8_t TouchSlider::getSwipeStatusFine() {
  int8_t swipeFineStatus = _swipeFineDownCount - _swipeFineUpCount;   // Calculate the swipe status as the difference between swipe-down and swipe-up counts

  // Reset the swipe counts after retrieving the swipe status
  _swipeFineUpCount = 0;
  _swipeFineDownCount = 0;

  return swipeFineStatus;
}


/**
 * @brief  Update the touch pads states
 *        This method is called periodically by a ticker
 */
void TouchSlider::update(TouchSlider* self) {
  self->CAP1208_Sensor->getTouchData(self->_SliderTouched);
  // self->printSliderTouched();

  bool padTouchedFound = false;
  int8_t firstTouchedIndex = -1;
  int8_t lastTouchedIndex = -1;
  uint8_t touchedPadCount = 0;

  // Check touch status and count touched pads
  checkSliderStatus(self, padTouchedFound, firstTouchedIndex, lastTouchedIndex, touchedPadCount);

  if (!padTouchedFound) { // Handle the cases when no pad is touched
    handleNoTouch(self);
  } else {  // Handle the case when at least one pad is touched
    handleTouch(self, firstTouchedIndex, lastTouchedIndex, touchedPadCount);
  }
}

/**
 * @brief Check the touch status of the slider pads.
 *
 * @param self Pointer to the TouchSlider instance.
 * @param padTouchedFound Variable to indicate if any pad is touched.
 * @param firstTouchedIndex Index of the first touched pad.
 * @param lastTouchedIndex Index of the last touched pad.
 * @param touchedPadCount Count of touched pads.
 */
void TouchSlider::checkSliderStatus(TouchSlider* self, bool& padTouchedFound, int8_t& firstTouchedIndex,
                                   int8_t& lastTouchedIndex, uint8_t& touchedPadCount) {

  for (uint8_t i = 0; i < self->_numSliderPins; ++i) {
    if (self->_SliderTouched[i]) {
    touchedPadCount++; // Increment the touched pad count if this pad is touched
    }
  }
  
  for (uint8_t i = 0; i < self->_numSliderPins; ++i) {  // Determine the first and last touched pad indices
    if (self->_SliderTouched[i]) {
      padTouchedFound = true;
      if (firstTouchedIndex == -1) {
        firstTouchedIndex = i;
      }
      lastTouchedIndex = i;
    }
    self->_lastValue = self->_actualValue;  // Store the last value for reference
  }
}

/**
 * @brief Handle cases when no pad is touched.
 *
 * @param self Pointer to the TouchSlider instance.
 */
void TouchSlider::handleNoTouch(TouchSlider* self) {
  for (uint8_t i = 0; i < self->_numSliderPins; ++i) {
    self->_sliderValue[i] = 0;  // Reset slider values and set actual value to 0
  }
  self->_actualValue = 0;
  self->firstTouch = true;

  if(self->_enableSwipeFine) {    // Check if that functionality Swipe Fine is active 
    // Increment swipe counts if the first pad touched was top or bottom
    if(self->firstPadTop) {
      self->_swipeFineUpCount++;
      if(self->_enablePrintSwipeStatus) LOGIB("SWIPE FINE UP");
    }
    if(self->firstPadBot) {
      self->_swipeFineDownCount++;
      if(self->_enablePrintSwipeStatus) LOGIR("SWIPE FINE DOWN");
    }
  }
  
  self->resetFirstTouches();
}

/**
 * @brief Handle cases when at least one pad is touched.
 *
 * @param self Pointer to the TouchSlider instance.
 * @param firstTouchedIndex Index of the first touched pad.
 * @param lastTouchedIndex Index of the last touched pad.
 * @param touchedPadCount Count of touched pads.
 */
void TouchSlider::handleTouch(TouchSlider* self, int8_t firstTouchedIndex, int8_t lastTouchedIndex, uint8_t touchedPadCount) {
  if(self->firstTouch == true) {  // Check if this is the first entry into this condition block
  if(self->_enablePrintSliderTouched) self->printSliderTouched();       // Check if _enablePrintSliderTouched is true for a Print SliderTouched[] 
    if(touchedPadCount == 1) {    // Check if only one pad is touched
      if (self->_SliderTouched[0] == true) {  
        self->firstPadBot = true;
        if(self->_enablePrintSwipeStatus) LOGIR("FIRST TOUCH BOT");
      }
      if(self->_SliderTouched[self->_numSliderPins - 1] == true) {
        self->firstPadTop = true;
        if(self->_enablePrintSwipeStatus) LOGIB("FIRST TOUCH TOP");
      }
    }
  }

  // Calculate slider values based on touched pads
  for (uint8_t i = 0; i < self->_numSliderPins; ++i) {
    if (i >= firstTouchedIndex && i <= lastTouchedIndex) {
      self->_sliderValue[i] = 0;
    } else if (i < firstTouchedIndex) {
      self->_sliderValue[i] = -1;
    } else if (i > lastTouchedIndex) {
      self->_sliderValue[i] = 1;
    }
  }
  self->analyzeGesture(self->_numSliderPins);   // Analyze the gesture based on the slider values
  self->firstTouch = false;
}

/**
 * @brief Analyze the slider touch pad states to detect a swipe up or down gesture.
 *
 * This function analyzes the states of the slider touch pads to detect swipe gestures (up or down).
 * It calculates the actual value based on slider values and compares it to the previous value to determine the gesture.
 * Detected gestures are logged for monitoring purposes.
 *
 * @param numSliders The number of slider touch pads to analyze.
 */
void TouchSlider::analyzeGesture(uint8_t numSliders) {
  _actualValue = 0;
  for (uint8_t i = 0; i < numSliders; ++i) {    // Calculate the actual value by summing slider values
    _actualValue += _sliderValue[i];
  }

  if (_actualValue != _lastValue && !firstTouch) {    // Check if there is no change or it's the first touch
    _swipeCount = _actualValue - _lastValue;    // Calculate the swipe count and determine the gesture
    if (_swipeCount > 0) {
      _sliderState = SWIPE_UP;
      _swipeUpCount++;
      resetFirstTouches();
      LOGIR("SWIPE_UP");
    } else if (_swipeCount < 0) {
      _sliderState = SWIPE_DOWN;
      _swipeDownCount++;
      resetFirstTouches();
      LOGIB("SWIPE_DOWN");
    } else {
      _sliderState = NO_CHANGE;
    }
  } else {
    _sliderState = NO_CHANGE;
  }
}

/**
 * @brief Reset first touch flags.
 */
void TouchSlider::resetFirstTouches() {
  firstPadBot = false;
  firstPadTop = false;
}


/**
 * @brief Print the states of the slider touch pads.
 *
 * This function prints the status of the slider touch pads, indicating whether each touch pad is currently touched or not.
 */
void TouchSlider::printSliderTouched() {
  String touchedStatus;
  for (uint8_t i = 0; i < _numSliderPins; i++) {
    touchedStatus += " " + String(_SliderTouched[i]);
  }

  log_i("Slider Touched Status: %s", touchedStatus.c_str());
}

/**
 * @brief Print the slider values.
 *
 * This function prints the values of the slider pins to the log. It generates a string representation
 * of the slider values and logs it for debugging or monitoring purposes.
 *
 * @param numSliders The number of slider values to print.
 */
void TouchSlider::printSliderValues(uint8_t numSliders) {
  String values;
  for (uint8_t i = 0; i < numSliders; ++i) {
    values += " " + String(_sliderValue[i]);
  }
  log_i("Slider values:%s", values.c_str());
}

/**
 * @brief Get the slider touched status.
 * 
 * @param sliderTouched: The array to store the slider touched status
 * @param numSliderPins: The number of slider pins
 * 
 * This function gets the slider touched status and stores it in the provided array. 
 */
void TouchSlider::getSliderTouched(bool sliderTouched[], uint8_t numSliderPins)
{
  for (uint8_t i = 0; i < numSliderPins; ++i) {
    sliderTouched[i] = _SliderTouched[i];
  }
}