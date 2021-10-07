# DAQ Simulator in C : Scrolling Display "HELLO"

## Contents

* [Overview](#Overview)
	* [Project Expansions](#Project-Expansions)
* [Installations](#Installations)
    * [VS-2017](#Visual-Studio-2017)
    * [DAQ Library](#DAQ-Library)
		* [Encoding](#Encoding)
		* [Functions](#Functions)
* [Scrolling Algorithm](#Scrolling-Algorithm)
	* [Starting Message](#Starting-Message)
	* [Complete Message](#Complete-Message)
	* [Separated Message](#Separated-Message)
		* [Counters](#Counters)
	* [Finishing Message](#Finishing-Message)
	* [Program End](#Program-End)
* [Demonstration](#Demonstration)
* [Credit](#Credit)

## Overview

This repository contains a **C** project developed using the **Data Acquisition (i.e. DAQ)** library in the <b>Visual Studio 2017 IDE</b>.

This program iteratively scrolls a message on a set of 8-segment displays.

### Project Expansions

The original implementation of this project was expanded to incorporate standard input/output, switch event functionality, and improve program performance through the reduction of refresh cycles.

In the interest of writing portable and reusable code, the source code was refactored to implement purposeful functions in separate source files. The comments in the **C** source files go into further detail of the program functionality.

## Installations

### Visual Studio 2017

The **C** source code is written to be compiled and run in the <b>Visual Studio 2017 IDE</b>. This <b>IDE</b> is also required for <b>DAQ</b> library installation on the <b>Windows OS</b>. The [(`Scrolling_Display_DAQ.sln`)](project/Scrolling_Display_DAQ.sln) solution file can be opened in the <b>Visual Studio</b> software.

<b>Configuration Properties</b> in the <b>VS Solution Explorer</b> :

<ul>
    <li><b>Configuration Properties->Debugging->Environment</b> :</br> <i>PATH=%PATH%;$(DAQLocalInstallPath)\bin64;$(DAQGlobalInstallPath)\bin64;$(LocalDebuggerEnvironment)</i></li>
    <li><b>C/C++->General->Additional Include Directories</b> :</br> <i>$(SolutionDir)\Scrolling_Display_DAQ\include;$(SolutionDir)\Scrolling_Display_DAQ\src;$(DAQLocalInstallPath)\include;$(DAQGlobalInstallPath)\include;%(AdditionalIncludeDirectories)</i></li>
    <li><b>Linker->General->Additional Library Directories</b> :</br> <i>$(DAQLocalInstallPath)\lib64;$(DAQGlobalInstallPath)\lib64;%(AdditionalLibraryDirectories)</i></li>
</ul>

<p align="center">
    <img src="Figures/VS_Configuration_Properties.JPG" width="50%" height="50%" title="Configuration Properties Window." >
</p>

### DAQ Library

The <b>DAQ</b> **C** library is installed to gain access to the simulator used in this program. I have included the [(`DAQlib`)](DAQlib) installation folder in the repository.

In order to gain access to the <b>DAQ</b> functions in our program, we must include the [(`DAQlib.h`)](project\DAQlib\include\DAQlib.h) header file in our code.

#### Encoding

The 8-segment displays in the <b>DAQ</b> library are driven by active-high outputs (i.e. set a bit to *0* to turn the segment **OFF** and *1* to turn the segment **ON**.)

We have encoded the relevant possible displays in the [(`main.h`)](project/Scrolling_Display_DAQ/include/main.h) and [(`counters.h`)](project/Scrolling_Display_DAQ/include/counters.h) header files as shown in the table below.

| 8-Segment Display | Code |
| ----------------- | ---- |
| H | 0b01101110 |
| E | 0b10011110 |
| L | 0b00011100 |
| O | 0b11111100 |
| - | ---------- |
| b | 0b00111110 |
| y | 0b01110110 |
| e | 0b11011110 |
| - | ---------- |
| 0 | 0b11111100 |
| 1 | 0b01100000 |
| 2 | 0b11011010 |
| 3 | 0b11110010 |
| 4 | 0b01100110 |
| 5 | 0b10110110 |
| 6 | 0b10111110 |
| 7 | 0b11100000 |
| 8 | 0b11111110 |
| 9 | 0b11110110 |

#### Functions

We must setup our simulator by calling the int `int setupDAQ(int setupNum);` function. We leave the <b>DAQ</b> configuration to the user to decide, but it's expected that a hardware module is unavailable.

Afterwards, we write message bytes to positions on the digital display by calling the `void displayWrite(int data, int position);` function. The `void delay(unsigned long ms);` function gives us the ability of periodically shifting the message on the digital displays.

In addition to the scrolling functionality, this program incorporates switch channels. We call the `int digitalRead(int channel);` function to read the current digital state of the switch channels. For our purposes, we use the switches to run and reset our scrolling display.

## Scrolling Algorithm

The functions which are used to scroll the message on the 8-segment displays are implemented in the [(`main.c`)](project/Scrolling_Display_DAQ/src/main.c) source file. These were developed from display cases determined prior to writing the source code.

### Starting Message

The `void shiftStartingMessage(uint8_t* message, uint8_t message_length);` function is used to implement the following display cases.

| Display Case | Digital Displays |
| - | ----------------------- |
| A | __ __ __ __ __ __ __ __ |
| B | __ __ __ __ __ __ __ H  |
| C | __ __ __ __ __ __ H  E  |
| D | __ __ __ __ __ H  E  L  |
| E | __ __ __ __ H  E  L  L  |

### Complete Message

The `void shiftCompleteMessage(uint8_t* message, uint8_t message_length, uint8_t counter);` function is used to implement the following display cases.

| Display Case | Digital Displays |
| - | ----------------------- |
| F | __ __ __ H  E  L  L  O  |
| G | __ __ H  E  L  L  O  __ |
| H | __ H  E  L  L  O  __ 9  |
| I | H  E  L  L  O  __ 9  __ |

For this example, our scrolling display counter is *9*.

### Separated Message

The `void shiftSeparatedMessage(uint8_t* message, uint8_t message_length, uint8_t counter);` function is used to implement the following display cases.

| Display Case | Digital Displays |
| - | ----------------------- |
| J | E  L  L  O  __ 9  __ H  |
| K | L  L  O  __ 9  __ H  E  |
| L | L  O  __ 9  __ H  E  L  |
| M | O  __ 9  __ H  E  L  L  |
| N | __ 9  __ H  E  L  L  O  |
| O | 9  __ H  E  L  L  O  __ |

For this example, our scrolling display counter is *9*.

#### Counters

The `void shiftCompleteMessage(...);` and `void shiftSeparatedMessage(...);` functions have been written to handle single digit and double digit counters. Based on the appropriate display position, they call the `void writeCounter(uint8_t counter, int8_t starting_position);` function from the [(`counters.c`)]((project/Scrolling_Display_DAQ/src/counters.c)) source file. The following display cases are implemented when the scrolling display counter is greater than *9*.

| Display Case | Digital Displays |
| - | ----------------------- |
| P | __ H  E  L  L  O  __ 1  |
| Q | H  E  L  L  O  __ 1  9  |
| R | E  L  L  O  __ 1  9  __ |
| S | L  L  O  __ 1  9  __ H  |
| T | L  O  __ 1  9  __ H  E  |
| U | O  __ 1  9  __ H  E  L  |
| V | __ 1  9  __ H  E  L  L  |
| W | 1  9  __ H  E  L  L  O  |
| X | 9  __ H  E  L  L  O  __ |

For this example, our scrolling display counter is *19*.

### Finishing Message

The `void shiftFinishingMessage(uint8_t* message, uint8_t message_length, uint8_t counter);` function is used to implement the following display cases.

| Display Case | Digital Displays |
| - | ----------------------- |
| Y | E  L  L  O  __ 1  9  __ |
| Z | L  L  O  __ 1  9  __ __ |
| a | L  O  __ 1  9  __ __ __ |
| b | O  __ 1  9  __ __ __ __ |
| c | __ 1  9  __ __ __ __ __ |
| d | 1  9  __ __ __ __ __ __ |
| e | 9  __ __ __ __ __ __ __ |
| f | __ __ __ __ __ __ __ __ |
| - | ----------------------- |

For this example, our scrolling display counter is *19*.

### Program End

The `void endDisplay(uint8_t* message, uint8_t message_length, uint8_t counter);` function is used to implement the following display cases. After the scrolling iterations of the <i>'HELLO'</i> message have finished, a <i>'byebye'</i> message is scrolled to conclude the program. This is done by calling the `void shiftStartingMessage(...);`, `void shiftCompleteMessage(...);`, and `void shiftFinishingMessage(...);` functions.


This call the `void writeCounter(uint8_t counter, int8_t starting_position);` function from the [(`counters.c`)]((project/Scrolling_Display_DAQ/src/counters.c)) source file.

| g | __ __ __ __ __ __ __ b  |
| h | __ __ __ __ __ __ b  y  |
| i | __ __ __ __ __ b  y  e  |
| j | __ __ __ __ b  y  e  b  |
| k | __ __ __ b  y  e  b  y  |
| - | ----------------------- |
| l | __ __ b  y  e  b  y  e  |
| m | __ b  y  e  b  y  e  __ |
| n | b  y  e  b  y  e  __ __ |
| - | ----------------------- |
| o | y  e  b  y  e  __ __ __ |
| p | e  b  y  e  __ __ __ __ |
| q | b  y  e  __ __ __ __ __ |
| r | y  e  __ __ __ __ __ __ |
| s | e  __ __ __ __ __ __ __ |
| t | 8. 8. 8. 8. 8. 8. 8. 8. |

## Demonstration

The videos in the [`demonstrations`](demonstrations) directory show the <b>DAQ</b> configuration and the program output on the software simulator. This directory is tracked using <b>Git LFS</b> due to size restrictions.</i> We have embedded low resolution compressed versions of these files below.

## Credit

The <b>DAQ</b> library is provided by <b>UBC ECE Department</b> for academic use.

The idea for this project was derived from <b>The University of British Columbia Electrical and Computer Engineering</b> undergraduate program. The <b>APSC 160 - Introduction to Computation</b> course requirement involves a final project which is a simplified scrolling display. This project was not used as a submission and was developed from scratch.

The source code was also refactored and heavily expanded after a formal introduction to <b>Pointers</b> in <b>CPSC 259 : Data Structures and Algorithms for Electrical Engineers</b> and an introduction to embedded development with microcontrollers.