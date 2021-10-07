/*
 * File: switches.h
 * Purpose: Contains Precompiler Directives, Enumerations
 *          and Function Prototypes for Switch Events in DAQ Programs.
 * Author: Muntakim Rahman
 * Date: 2021-10-06
 */

#ifndef SWITCHES_H
#define SWITCHES_H

/*******************/
/* Include Headers */
/******************/

#include <stdint.h>

/* Provided by UBC ECE Department. */
#include "DAQlib.h"

/**************************/
/* Precompiler Directives */
/*************************/

#define FALSE 0
#define TRUE 1

/****************/
/* Enumerations */
/****************/

typedef enum {
    RESET_CHANNEL = 0,
    RUN_CHANNEL = 1
} SWITCH_CHANNELS;

/***********************/
/* Function Prototypes */
/***********************/

void configSwitches(void);

uint8_t resetDAQ(void);

#endif