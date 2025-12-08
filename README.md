# Light_Chaser
*   Mini version of light chasing arcade game, inspired by Innovative Concepts in Entertainment's Cyclone and similar machines.

*   Developed in embedded C using STM32CubeIDE

## Overview
## Hardware
1. MCU
    *   NUCLEO-L476RG development board for its STM32L476RG Microprocessor (ARM Cortex-M4)
2. Circuit
    *   Overview ![System Architecture Diagram](https://github.com/addysfsu/Light_Chaser/blob/main/img/image-1.png?raw=true)
    *   LTSpice Schematic ![Direct Circuit](https://github.com/addysfsu/Light_Chaser/blob/main/img/image-2.png?raw=true)
## Software
1. Required header files
    *   Be sure you have CMSIS package in your project folder
    *   ![Screenshot of project file structure, showing subfolder 'CMSIS'](https://github.com/addysfsu/Light_Chaser/blob/main/img/image.png?raw=true)
2. Software Logic
    *   ![UML-1](https://github.com/addysfsu/Light_Chaser/blob/main/img/image-3.png?raw=true)
    *   ![UML-2](https://github.com/addysfsu/Light_Chaser/blob/main/img/image-4.png?raw=true)
## 3D Print
1. 3D model files
    *   The 3D model .stl files were created in Shapr3D
    *   There is a plate for holding the LEDs and buttons, that snaps into a case. An existing clear plastic bubble was repurposed for our build, however any 71mm diameter hemisphere will do.
    *   For the LED holding mounts, [this](https://www.printables.com/model/625992-5mm-led-holder) print was used