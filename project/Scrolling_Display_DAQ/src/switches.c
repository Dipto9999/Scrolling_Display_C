/*
 * File: buttons.c
 * Purpose: To Add Switch Functionality to the Scrolling Display DAQ Program.
 * 		Input: Current Digital State of Switch Channel.
 *		Output: Pauses Program Execution or Signals Scrolling Display to Reset Itself.
 * Author: Muntakim Rahman
 * Date: 2021-10-06
 */

#include "switches.h"

/*
 * Checks that the DAQ Switch Channels Are Configured With Default Settings.
 *
 * PARAM: VOID
 * PRE: DAQ module for scrolling display program has been initialized.
 * POST: NULL (no side-effects)
 * RETURN: VOID
 */
void configSwitches(void) {
	uint8_t reset_event = FALSE;
	uint8_t run_event = FALSE;

    do {
		run_event = digitalRead(RUN_CHANNEL);
		reset_event = digitalRead(RESET_CHANNEL);
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
		run_event = digitalRead(RUN_CHANNEL);

		while (digitalRead(RESET_CHANNEL) == TRUE) reset_event = TRUE;
		if (reset_event == TRUE) return TRUE;
	} while (run_event == FALSE);

	return FALSE;
}
