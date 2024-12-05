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

This FeatherWing is designed for the **[Adafruit ESP32 Feather V2](https://www.adafruit.com/product/5400)**, which can be purchased from Adafruit's website along with other Wing modules. It stands out as one of the best development boards due to its easy LiPo battery integration and the ability to stack various pre-designed boards.  

<p align="center">
  <img src="https://github.com/user-attachments/assets/b980839f-5dd5-40b3-a23a-6a3e57ba48af" alt="Feather_V2" style="width: 40%; height: auto;">
  <br>
  <em>Adafruit ESP32 Feather V2</em>
</p>


## Examples

## Features

## Features  

The module integrates the following electronic components:  
- **[CAP1208](https://ww1.microchip.com/downloads/en/DeviceDoc/00001570C.pdf)**: An 8-channel capacitive touch sensor controller with programmable sensitivity, designed for touch sensor applications.  
- **[SK6812MINI-E](https://cdn-shop.adafruit.com/product-files/4960/4960_SK6812MINI-E_REV02_EN.pdf)**: 8 NeoPixels, ideal for not interfering with sliding gestures due to their compact design.  

Communication is handled via I2C, using **GPIO22** and **GPIO20** by default on the ESP32.

<p align="center">
  <img src="https://github.com/user-attachments/assets/675564c5-56dc-4473-8a33-61696ab01ec6" alt="Feather_V2_PinOut" style="width: 65%; height: auto;">
  <br>
  <em>Adafruit ESP32 Feather V2 - PinOut</em>
</p>

A **RESET button** is included, and a jumper will be added soon to allow selection between RESET or a GPIO pin for use as an interrupt.  

The NeoPixels are powered automatically via USB or the LiPo battery on **VBAT**:  
- **USB** power: 4.3V  
- **LiPo battery**: Voltage ranges from 4.2V to 3.5V depending on the battery charge.  

The manufacturer of the **SK6812MINI-E** specifies an operating voltage of **3.7V to 5.5V**. Therefore, it is recommended not to use a battery discharged below 3.7V, as the colors may become inaccurate.  

<p align="center">
  <img src="https://github.com/user-attachments/assets/084af5eb-f819-4a5e-a3dc-85cae4f56c0c" alt="SK6812MINI-E_Data" style="width: 40%; height: auto;">
  <br>
  <em>SK6812MINI-E - Electrical Parameters</em>
</p>



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


