/*
 * File: counters.c
 * Purpose: To Add Counter Functionality to the Scrolling Display DAQ Program.
 * 		Input: Final Value of a Counter
 *		Output: Current Value of Counter on Specified Digital Displays.
 * Author: Muntakim Rahman
 * Date: 2021-10-04
 */

#include "counters.h"

/*
 * Writes the Intended Counter to the LED Displays.
 *
 * PARAM: number is a uint8_t to be written to the led displays.
 *		  display_offset is a uint8_t representing the offset from the led display at DISPLAY_START.
 * PRE:	VALUE_MIN <= counter < VALUE_COUNTER_MAX, VALUE_MIN <= display_offset <= MAX_DISPLAY
 * POST: counter is displayed on digital displays starting from display_offset.
 * RETURN: VOID
 */
void writeCounter(uint8_t counter, uint8_t display_offset) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t current_digit = FALSE;
	uint8_t current_position = display_offset;

	/* Write Counter to Digital Display. */
	do {
		current_digit = counter % (VALUE_DIGIT_MAX + 1);
		writeDigit(current_digit, current_position);

		/* Prepare to Display Next Digit. */
		counter /= (VALUE_DIGIT_MAX + 1);
	} while (++current_position < DISPLAY_END && counter != VALUE_MIN);

	return;
}


/*
 * Writes the Intended Digit to the Digital Display Position.
 *
 * PARAM:	digit is a uint8_t to be written to the digital displays;
 * 			position is a uin8_t indicating the digital display position at which the digit is written.
 * PRE:	VALUE_MIN <= digit <= VALUE_DIGIT_MAX, DISPLAY_START <= position < DISPLAY_END
 * POST: digit is displayed on led display at position.
 * RETURN:	VOID
 */
void writeDigit(uint8_t digit, uint8_t position) {
	switch (digit) {
		case 0 :
			displayWrite(DISPLAY_0, position);
			break;
		case 1 :
			displayWrite(DISPLAY_1, position);
			break;
		case 2 :
			displayWrite(DISPLAY_2, position);
			break;
		case 3 :
			displayWrite(DISPLAY_3, position);
			break;
		case 4 :
			displayWrite(DISPLAY_4, position);
			break;
		case 5 :
			displayWrite(DISPLAY_5, position);
			break;
		case 6 :
			displayWrite(DISPLAY_6, position);
			break;
		case 7 :
			displayWrite(DISPLAY_7, position);
			break;
		case 8 :
			displayWrite(DISPLAY_8, position);
			break;
		case 9 :
			displayWrite(DISPLAY_9, position);
			break;
		/* Digit Isn't In the Specified Range. */
		default:
			break;
	}
	return;
}