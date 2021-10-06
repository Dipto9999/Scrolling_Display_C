/*
 * File: buttons.h
 * Purpose: Contains Precompiler Directives, Enumerations
 *          and Function Prototypes for Dealing With Buttons in DAQ Programs.
 * Author: Muntakim Rahman
 * Date: 2021-10-04
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

#define RESET_BUTTON 0
#define RUN_BUTTON 1

/***********************/
/* Function Prototypes */
/***********************/

void configButtons(void);

uint8_t resetDAQ(void);

#endif