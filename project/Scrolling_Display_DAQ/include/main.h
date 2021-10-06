/*
 * File: main.h
 * Purpose: Contains Constants, Function Prototypes, and Phrase Array.
 * Author: Muntakim Rahman
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

#define LAGGING_COUNT_OFFSET 2
#define LEADING_COUNT_OFFSET 1

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
extern uint8_t phrase_message[];
extern uint8_t ending_message[];

/***********************/
/* Function Prototypes */
/***********************/

int8_t configDAQ(void);

void scrollDisplay(uint8_t* message, uint8_t message_length, int8_t number_scrolls);

void shiftStartingMessage(uint8_t* message, uint8_t message_length);
void shiftFinishingMessage(uint8_t* message, uint8_t message_length, uint8_t loop_count);

void shiftCompleteMessage(uint8_t* message, uint8_t message_length, uint8_t loop_count);

void shiftSeparatedMessage(uint8_t* message, uint8_t message_length, uint8_t loop_count);

void endDisplay(uint8_t* message, uint8_t message_length, uint8_t loop_count);

void refreshDisplay(uint8_t refresh_start, uint8_t refresh_end, uint8_t turn_on);

void handleError(char* message);

#endif
