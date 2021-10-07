/*
 * File: buttons.h
 * Purpose: Contains Precompiler Directives, Enumerations
 *          and Function Prototypes for Button Events in DAQ Programs.
 * Author: Muntakim Rahman
 * Date: 2021-10-06
 */

#ifndef BUTTONS_H
#define BUTTONS_H

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
    RESET_BUTTON = 0,
    RUN_BUTTON = 1
} BUTTONS;

/***********************/
/* Function Prototypes */
/***********************/

void configButtons(void);

uint8_t resetDAQ(void);

#endif