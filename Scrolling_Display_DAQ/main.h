/*
 * File: main.h
 * Purpose: Contains Constants, Function Prototypes, and Phrase Array.
 * Author: Muntakim Rahman
 * Student #: 71065221
 * Lab Section:	L2M
 * Date: 2021-10-04
 */

#ifndef MAIN_H
#define MAIN_H

/*******************/
/* Include Headers */
/******************/

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <Windows.h>

#include "counters.h"

/**************************/
/* Precompiler Directives */
/*************************/

#define PROGRAM_ERROR -1

#define BUFFSIZE 512

#define PHRASE_LENGTH 5
#define ENDING_LENGTH 6

/* Active High Signals used to Turn On 7-Segment Displays. */
#define DISPLAY_H 0b01101110
#define DISPLAY_E 0b10011110
#define DISPLAY_L 0b00011100
#define DISPLAY_O 0b11111100

#define DISPLAY_b 0b00111110
#define DISPLAY_y 0b01110110
#define DISPLAY_e 0b11011110

#define MAX_OFFSET (DISPLAY_END - PHRASE_LENGTH)
#define BROKEN_CASES (PHRASE_LENGTH - 1)

#define LAGGING_COUNT_OFFSET 2
#define LEADING_COUNT_OFFSET 1

#define COUNT_APPEARS (MAX_OFFSET - 1)

#define COMPLETE_MESSAGE_LENGTH (PHRASE_LENGTH + LAGGING_COUNT_OFFSET)
#define COMPLETE_MESSAGE_END (DISPLAY_END - COMPLETE_MESSAGE_LENGTH)

/****************/
/* Enumerations */
/****************/

/* Use DAQ Simulator or Device for Scrolling Display. */
typedef enum {
	DEVICE_CONFIG = 0,
	SIMULATOR_CONFIG = 4
} DAQ_CONFIGS;

/***************************/
/**** Declare Variables ****/
/***************************/

extern char input_buff[];
extern char output_buff[];

/* Initialize Array with Display 'HELLO' According to DAQ Definitions. */
extern uint8_t phrase[];
extern uint8_t end_message[];

/***********************/
/* Function Prototypes */
/***********************/

void scrollDisplay(uint8_t number_scrolls);

void shiftIntactPhrase(uint8_t loop_count);
void shiftIntactLoopCount(uint8_t loop_count, uint8_t phrase_offset);

void shiftBrokenPhrase(uint8_t loop_count);

void refreshDisplay(uint8_t refresh_start, uint8_t refresh_end, uint8_t turn_on);

void endDisplay(uint8_t loop_count);

void handleError(char* message);

#endif