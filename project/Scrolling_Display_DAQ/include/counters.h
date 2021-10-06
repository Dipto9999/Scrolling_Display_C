/*
 * File: counters.h
 * Purpose: Contains Precompiler Directives, Enumerations
 *          and Function Prototypes for Dealing With Counters in DAQ Programs.
 * Author: Muntakim Rahman
 * Date: 2021-10-04
 */

#ifndef COUNTERS_H
#define COUNTERS_H

/*******************/
/* Include Headers */
/******************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <Windows.h>

/* Provided by UBC ECE Department. */
#include "DAQlib.h"

/**************************/
/* Precompiler Directives */
/*************************/

#define FALSE 0
#define TRUE 1

#define NUMBER_DISPLAYS 8

/* Active High Signals used to Turn On 7-Segment Displays. */
#define DISPLAY_OFF 0b00000000
#define DISPLAY_ON 0b11111111

#define DISPLAY_0 0b11111100
#define DISPLAY_1 0b01100000
#define DISPLAY_2 0b11011010
#define DISPLAY_3 0b11110010
#define DISPLAY_4 0b01100110
#define DISPLAY_5 0b10110110
#define DISPLAY_6 0b10111110
#define DISPLAY_7 0b11100000
#define DISPLAY_8 0b11111110
#define DISPLAY_9 0b11110110

#define VALUE_MIN 0
#define VALUE_DIGIT_MAX 9
#define VALUE_COUNTER_MAX 99

/****************/
/* Enumerations */
/****************/

typedef enum {
    DELAY_STANDARD = 1000,
    DELAY_SHORT = 500
} DELAY_LENGTHS;

/***********************/
/* Function Prototypes */
/***********************/

void writeCounter(uint8_t counter, int8_t display_offset);
void writeDigit(uint8_t digit, int8_t position);

#endif