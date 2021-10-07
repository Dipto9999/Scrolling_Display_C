/*
 * File: counters.c
 * Purpose: To Add Counter Functionality to the Scrolling Display DAQ Program.
 * 		Input: Total Number of Scrolling Display Iterations.
 *		Output: Current Number of Scrolling Display Iterations.
 * Author: Muntakim Rahman
 * Date: 2021-10-06
 */

#include "counters.h"

/*
 * Writes the Intended Counter to the Digital Displays.
 *
 * PARAM: counter is a uint8_t to be written to the digital displays;
 *		  starting_position is an int8_t representing the index of the digital display
 * 		  at which the least significant digit of the counter is written.
 * PRE:	VALUE_MIN <= counter <= VALUE_COUNTER_MAX
 * POST: counter is displayed on digital displays from starting_position onwards.
 * RETURN: VOID
 */
void writeCounter(uint8_t counter, int8_t starting_position) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t current_digit = FALSE;
	int8_t display_position = starting_position;

	if (starting_position >= NUMBER_DISPLAYS) return;

	/* Write Counter Value to Digital Displays. */
	do {
		/* Write Least Significant Counter Digit. */
		current_digit = counter % (VALUE_DIGIT_MAX + 1);

		/* Check if Display Position is Within Range. */
		if (display_position >= VALUE_MIN) writeDigit(current_digit, display_position);

		/* Prepare to Display Next Counter Digit. */
		counter /= (VALUE_DIGIT_MAX + 1);
	} while (++display_position < NUMBER_DISPLAYS && counter != VALUE_MIN);

	return;
}

/*
 * Writes the Intended Counter Digit to the Digital Displays.
 *
 * PARAM: digit is a uint8_t to be written to the digital displays;
 *		  display_position is an int8_t representing the index of the digital display
 * 		  at which the counter digit is written.
 * PRE:	VALUE_MIN <= digit <= VALUE_DIGIT_MAX, VALUE_MIN <= digital_position < NUMBER_DISPLAYS
 * POST: counter digit is displayed on digital displays at display_position.
 * RETURN: VOID
 */
void writeDigit(uint8_t digit, int8_t display_position) {
	switch (digit) {
		case 0 :
			displayWrite(DISPLAY_0, display_position);
			break;
		case 1 :
			displayWrite(DISPLAY_1, display_position);
			break;
		case 2 :
			displayWrite(DISPLAY_2, display_position);
			break;
		case 3 :
			displayWrite(DISPLAY_3, display_position);
			break;
		case 4 :
			displayWrite(DISPLAY_4, display_position);
			break;
		case 5 :
			displayWrite(DISPLAY_5, display_position);
			break;
		case 6 :
			displayWrite(DISPLAY_6, display_position);
			break;
		case 7 :
			displayWrite(DISPLAY_7, display_position);
			break;
		case 8 :
			displayWrite(DISPLAY_8, display_position);
			break;
		case 9 :
			displayWrite(DISPLAY_9, display_position);
			break;
		/* Counter Digit Isn't In the Specified Range. */
		default:
			break;
	}
	return;
}