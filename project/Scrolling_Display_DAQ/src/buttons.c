/*
 * File: buttons.c
 * Purpose: To Add Button Functionality to the Scrolling Display DAQ Program.
 * 		Input: Current Read State of Button.
 *		Output: Pauses Program Execution or Signals Scrolling Display to Reset Itself.
 * Author: Muntakim Rahman
 * Date: 2021-10-06
 */

#include "buttons.h"

/*
 * Checks that the DAQ Buttons Are Configured With Default Settings
 *
 * PARAM: VOID
 * PRE: DAQ module for scrolling display program has been initialized.
 * POST: NULL (no side-effects)
 * RETURN: VOID
 */
void configButtons(void) {
	uint8_t reset_event = FALSE;
	uint8_t run_event = FALSE;

    do {
		run_event = digitalRead(RUN_BUTTON);
		reset_event = digitalRead(RESET_BUTTON);
	} while (run_event == FALSE || reset_event == TRUE);
}

/*
 * Pause Scrolling Display Program To Check If Reset Event Has Occurred.
 *
 * PARAM: VOID
 * PRE: DAQ module for scrolling display program has been initialized.
 * POST: NULL (no side-effects)
 * RETURN: TRUE if reset event has occurred; otherwise FALSE.
 */
uint8_t resetDAQ(void) {
	uint8_t run_event = FALSE;
	uint8_t reset_event = FALSE;

	do {
		run_event = digitalRead(RUN_BUTTON);

		while (digitalRead(RESET_BUTTON) == TRUE) reset_event = TRUE;
		if (reset_event == TRUE) return TRUE;
	} while (run_event == FALSE);

	return FALSE;
}
