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
 *		  display_offset is a int8_t representing the offset from the led display at VALUE_MIN.
 * PRE:	VALUE_MIN <= counter < VALUE_COUNTER_MAX, VALUE_MIN <= display_offset <= MAX_DISPLAY
 * POST: counter is displayed on digital displays starting from display_offset.
 * RETURN: VOID
 */
void writeCounter(uint8_t counter, int8_t display_position) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t current_digit = FALSE;
	int8_t current_position = display_position;

	if (display_position >= NUMBER_DISPLAYS) return;

	/* Write Counter to Digital Display. */
	do {
		current_digit = counter % (VALUE_DIGIT_MAX + 1);
		if (current_position >= VALUE_MIN) writeDigit(current_digit, current_position);

		/* Prepare to Display Next Digit. */
		counter /= (VALUE_DIGIT_MAX + 1);
	} while (++current_position < NUMBER_DISPLAYS && counter != VALUE_MIN);

	return;
}


/*
 * Writes the Intended Digit to the Digital Display Position.
 *
 * PARAM:	digit is a uint8_t to be written to the digital displays;
 * 			position is a in8_t indicating the digital display position at which the digit is written.
 * PRE:	VALUE_MIN <= digit <= VALUE_DIGIT_MAX, VALUE_MIN <= position < NUMBER_DISPLAYS
 * POST: digit is displayed on led display at position.
 * RETURN:	VOID
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
		/* Digit Isn't In the Specified Range. */
		default:
			break;
	}
	return;
}