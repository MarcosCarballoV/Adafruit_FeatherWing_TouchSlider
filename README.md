# Adafruit FeatherWing TouchSlider - CAP1208 and NeoPixels
[![Altium Designer](https://img.shields.io/badge/Altium-24.0-blue.svg)](https://www.altium.com/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-5.2.1-blue.svg)](https://platformio.org/)
[![Version](https://img.shields.io/badge/Version-1.0.0-brightgreen.svg)](https://github.com/MarcosCarballoV/Adafruit_FeatherWing_TouchSlider)

## Overview
FeatherWing TouchSlider is an expansion board for the Adafruit ESP32 Feather V2 featuring the CAP1208 chip with 8 touch pads arranged as a slider. Each pad includes a NeoPixel for visual feedback, perfect for interactive interfaces and dynamic effects.

<p align="center">
  <img src="https://github.com/user-attachments/assets/3bf3efa0-4e80-4d0e-8736-3ef748b1a928" alt="Render_Top" style="width: 40%; height: auto;">
  <br>
  <em>FeatherWing TouchSlider - Render Top</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/0ab96d65-2c6a-4686-97d5-e164bc259dd0" alt="Render_Bot" style="width: 40%; height: auto;">
  <br>
  <em>FeatherWing TouchSlider - Render Bot</em>
</p>

## Supported Modules

## Supported Modules  

This FeatherWing is designed for the **[Adafruit ESP32 Feather V2](https://www.adafruit.com/product/5400)**, which can be purchased from Adafruit's website along with other Wing modules. It stands out as one of the best development boards due to its easy LiPo battery integration and the ability to stack various pre-designed boards.  

<p align="center">
  <img src="https://github.com/user-attachments/assets/b980839f-5dd5-40b3-a23a-6a3e57ba48af" alt="Feather_V2" style="width: 40%; height: auto;">
  <br>
  <em>Adafruit ESP32 Feather V2</em>
</p>


## Examples

## Features

## Why This Module

### Advantages:

## Authors

- [@MarcosCarballoV](https://github.com/MarcosCarballoV)
 
## Dependencies

This library depends on:


## Setup Instructions

The steps to install this library depend on the IDE you are using. PlatformIO is recommended, but the library can also be used with the plain Arduino IDE.

### PlatformIO (Recommended)

1. **Add the Library:**
   - Open the `platformio.ini` file in your project.
   - Add the following line to include the TouchSlider and CAP1208 library:
     ```ini
     lib_deps =
       https://github.com/MarcosCarballoV/Adafruit_FeatherWing_TouchSlider/Library
     ```

2. **Include the Library in Your Code:**
   - In your main `.cpp` or `.ino` file, include the TouchSlider and CAP1208 library:
     ```cpp
     #include <TouchSlider.h>
     #include <CAP1208.h>
     ```

3. **Upload Your Code:**
   - Write your code using the TouchSlider and CAP1208 library.
   - Connect your ESP32 board to your computer.
   - Click the upload button in PlatformIO to compile and upload your code to the board.

### Arduino IDE

1. **Install the TouchSlider Library:**
   - Download the TouchSlider and CAP1208 library from the [GitHub repository](https://github.com/MarcosCarballoV/Adafruit_FeatherWing_TouchSlider/Library).
   - Open the Arduino IDE.
   - Go to `Sketch` > `Include Library` > `Add .ZIP Library...`.
   - Select the downloaded TouchSlider library ZIP file to install it.

2. **Include the Library in Your Code:**
   - In your `.ino` file, include the TouchSlider and CAP1208 library:
     ```cpp
     #include <TouchSlider.h>
     #include <CAP1208.h>
     ```

3. **Upload Your Code:**
   - Write your code using the TouchSlider library.
   - Connect your ESP32 board to your computer.
   - Select the correct board and port from `Tools` > `Board` and `Tools` > `Port`.
   - Click the upload button to compile and upload your code to the board.

## Get Started

### Includes and Definitions
To use this library in your project, you need to include the following headers and set up the touch slider.

### Create Logic/Function for Each Gesture
It is necessary to create a function that will be repeatedly called by a timer configured in the main program.

### Configure the TouchSlider and Start
In your setup, or any other part of the main program that only runs once, use the enable/disable functions to configure the features of this library, such as printing messages or using touch buttons. After configuring, start the continuous reading of the touch slider with `touchSlider.start();`. This reading occurs every 50ms by default but can be changed within the library.

### Implement Rest of Your Code
The updateTouchSlider() function will be called every SLIDER_INTERVAL (In this example, 200 ms), independently of the rest of the code

## Documentation


