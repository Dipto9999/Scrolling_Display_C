/*
 * File : main.c
 * Purpose:	To Implement a Scrolling Phrase on an LED Screen Consisting of 8 Custom 7-Segment Displays
 * 	and 2 Toggle Switches.
 * 		Input: Scrolling Configuration
 *		Output:	Scrolling Message on LED screen
 *	UBC APSC Data Acquisition (DAQ) C Library Installed to Make Use of the Given
 * 	Functions and Simulate the Scrolling Display.
 * Author: Muntakim Rahman
 * Date: 2021-05-28
 */

#include "main.h"

/* Static Variables */

static uint8_t phrase[PHRASE_LENGTH] = {
    DISPLAY_H,
	DISPLAY_E,
	DISPLAY_L,
	DISPLAY_L,
	DISPLAY_O
};

static uint8_t end_message[ENDING_LENGTH] = {
	DISPLAY_b,
	DISPLAY_y,
	DISPLAY_e,
	DISPLAY_b,
	DISPLAY_y,
	DISPLAY_e
};

static char input_buff[BUFFSIZE];
static char input_extra[BUFFSIZE];
static char output_buff[BUFFSIZE];

/*
 * Main Function Drives the Program. The Scrolling Display is Shown
 * Only if the DAQ is Configured Appropriately.
 * PRE: NULL (no pre-conditions)
 * POST: NULL (no side-effects)
 * RETURN: VOID
 */
void main(void) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t display_config = FALSE;
	uint8_t number_scrolls = FALSE;

	/* DAQ Configuration Message */

	strcpy_s(output_buff, BUFFSIZE, "Enter the Configuration Type: (");

	input_buff[0] = '0' + DEVICE_CONFIG;
	strncat_s(output_buff, BUFFSIZE, input_buff, sizeof(char));
	strcat_s(output_buff, BUFFSIZE, " for the Device, ");

	input_buff[0] = '0' + SIMULATOR_CONFIG;
	strncat_s(output_buff, BUFFSIZE, input_buff, sizeof(char));
	strcat_s(output_buff, BUFFSIZE, " for the Simulator): ");

	/* Print to Standard Output. */
	fprintf(stdout, output_buff);

	/* Check for Valid Input */
	if (!fgets(input_buff, BUFFSIZE, stdin))
		handleError("\n\nExiting Program...\n\n");
	else if (sscanf_s(input_buff, "%d%s", (int*)&display_config, input_extra, BUFFSIZE) != TRUE)
		handleError("\n\nInvalid Input: Did Not Enter Integer...\n\n");
	else if (display_config != DEVICE_CONFIG && display_config != SIMULATOR_CONFIG)
		handleError("\n\nInvalid Display Configuration...\n\n");

	/* Scroll Limit Message */

	input_buff[0] = '0' + VALUE_DIGIT_MAX;
	strcpy_s(output_buff, BUFFSIZE, "Number of Iterations (Must Be Less Than or Equal To ");
	strncat_s(output_buff, BUFFSIZE, input_buff, sizeof(char));
	strcat_s(output_buff, BUFFSIZE, ") : ");

	/* Print to Standard Output. */
	fprintf(stdout, output_buff);

	if (!fgets(input_buff, BUFFSIZE, stdin))
		handleError("\n\nExiting Program...\n\n");
	else if (sscanf_s(input_buff, "%d%s", (int*)&number_scrolls, input_extra, BUFFSIZE) != TRUE)
		handleError("\n\nInvalid Input: Did Not Enter Integer...\n\n");
	else if ((number_scrolls < VALUE_MIN) || (number_scrolls > VALUE_COUNTER_MAX))
		handleError("\n\nInvalid Scrolling Configuration...\n\n");
	else setupDAQ(display_config);

	scrollDisplay(number_scrolls);
	endDisplay(number_scrolls);

	refreshDisplay(DISPLAY_START, DISPLAY_END, TRUE);

	/* System Command Forces the System to Pause Before Closing Executable Window. */
	system("PAUSE");
	exit(TRUE);
}

/*
 * Scrolls Phrase on Custom LED Display.
 *
 * PARAM: number_scrolls is a uint8_t which represents the number
 * 		  of iterations for the scrolling display.
 * PRE: DAQ module configured for either device or simulator.
 * POST: scrolling display is shown to user.
 * RETURN: VOID
 */
void scrollDisplay(uint8_t number_scrolls) {
	uint8_t loop_count = FALSE;

	/* Implement Code For Scrolling Display. */
	while (continueSuperLoop()) {
		refreshDisplay(PHRASE_LENGTH, DISPLAY_END, FALSE);

		/* Cases with Intact Phrase Shifting Through Displays. */
		shiftIntactPhrase(loop_count);

		if (++loop_count < number_scrolls) shiftBrokenPhrase(loop_count);
		else return;
	}
}

/*
 * Phrase is Individually Shifted Off the Digital Displays.
 *
 * PARAM:     loop_count is a uint8_t representing the current loop iteration.
 * PRE:       0 <= loop_count <= VALUE_DIGIT_MAX;
 *            phrase occupies digital display at DISPLAY_START ... digital display at PHRASE_LENGTH.
 * POST:      all digital displays are turned off.
 * RETURN:    VOID
 */
void endDisplay(uint8_t loop_count) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t case_index = FALSE;
	uint8_t phrase_index = FALSE;

	uint8_t relative_phrase_position = FALSE;
	uint8_t count_position = FALSE;
	uint8_t display_position = FALSE;

	uint8_t exceeding_offset = FALSE;

	/* Individually Shift Phrase Off Digital Displays. */
	for (case_index = 0; case_index < COMPLETE_MESSAGE_LENGTH; case_index++) {
		exceeding_offset = case_index + MAX_OFFSET;
		count_position = (DISPLAY_END - COMPLETE_MESSAGE_LENGTH) + case_index;

		for (display_position = (DISPLAY_END - 1); display_position >= COMPLETE_MESSAGE_END; display_position--) {
			relative_phrase_position = display_position - exceeding_offset;
			phrase_index = (PHRASE_LENGTH - 1) - relative_phrase_position;

			/* Write Phrase to the Appropriate Display Positions. */
			if (phrase_index < PHRASE_LENGTH) displayWrite(phrase[phrase_index], display_position);
			else if (display_position == count_position) writeCounter(loop_count, display_position);
			else displayWrite(DISPLAY_OFF, display_position);
		}
		Sleep(DELAY_SHORT);
	}
	return;
}

/*
 * Turns On/Off Unused Digital Displays.
 *
 * PARAM: refresh_start and refresh_end are uint8_ts which represent
 * 		  the left and right bounds of displays to be turned off respectively.
 * PRE:	DISPLAY_START <= refresh_start < refresh_end <= DISPLAY_END
 * POST: digital display at refresh_start .. led display at refresh_end
 * 		 are all turned off.
 * RETURN: VOID
 */
void refreshDisplay(uint8_t refresh_start, uint8_t refresh_end, uint8_t turn_on) {
	for (uint8_t refresh_index = refresh_start; refresh_index < refresh_end; refresh_index++) {
		if (turn_on == FALSE) displayWrite(DISPLAY_OFF, refresh_index);
		else if (turn_on == TRUE) displayWrite(DISPLAY_ON, refresh_index);
	}
}

/*
 * Intact Phrase is Written to Appropriate Digital Displays to Create Scrolling Effect.
 *
 * PARAM: loop_count is an uint8_t representing the current loop iteration.
 * PRE: VALUE_MIN <= loop_count <= VALUE_DIGIT_MAX, 0 <= refresh_start <= refresh_end <= DISPLAY_END
 * POST: phrase occupies digital display at PHRASE_LENGTH .. digital display at DISPLAY_END.
 * RETURN: VOID
 */
void shiftIntactPhrase(uint8_t loop_count) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t phrase_index = FALSE;

	uint8_t relative_phrase_position = FALSE;
	uint8_t display_position = FALSE;

	uint8_t phrase_offset = FALSE;

	/* Shift Phrase Until The Left Edge of the Digital Displays. */
	while (phrase_offset <= MAX_OFFSET) {
		/* Write Phrase to Digital Displays. */
		for (relative_phrase_position = 0; relative_phrase_position < PHRASE_LENGTH; relative_phrase_position++) {

			/* Turn Off all Displays Surrounding the Phrase. */
			refreshDisplay(DISPLAY_START, phrase_offset, FALSE);
			refreshDisplay(phrase_offset + PHRASE_LENGTH, DISPLAY_END, FALSE);

			/*
			 * Phrase Indices Should Correspond to Appropriate Display Positions (i.e OccupiedMid
			 * Portion of Design). The Relative Phrase Position is Mirror to the Array Indices.
			 *
			 * phrase[PHRASE_LENGTH - 1] is written in the 1st relative phrase position.
			 * phrase[PHRASE_LENGTH - 2] is written in the 2nd relative phrase position.
			 * ...
			 * phrase[1] is displayed in the 2nd last position.
			 * phrase[0] is displayed in the last position.
			*/
			phrase_index = (PHRASE_LENGTH - 1) - relative_phrase_position;

			/* Shift Phrase Leftwards */
			display_position = relative_phrase_position + phrase_offset;

			displayWrite(phrase[phrase_index], display_position);
		}

		shiftIntactLoopCount(loop_count, phrase_offset);
		Sleep(DELAY_SHORT);

		/* Prepare to Shift Phrase. */
		phrase_offset++;
	}
}

void shiftIntactLoopCount(uint8_t loop_count, uint8_t phrase_offset) {
	uint8_t count_position = FALSE;

	if (phrase_offset >= COUNT_APPEARS) {
	    /* Actual Loop Count Remains Unchanged. */
		count_position = phrase_offset - LAGGING_COUNT_OFFSET;
		writeCounter(loop_count + 1, count_position);
	}
	else if (loop_count > VALUE_MIN) {
		count_position = (phrase_offset + PHRASE_LENGTH) + LEADING_COUNT_OFFSET;
		writeCounter(loop_count, count_position);
	}
}

/*
 * Broken Phrase is Written to Appropriate Digital Displays to Create Scrolling Effect.
 *
 * PARAM: loop_count is a uint8_t representing the current loop iteration.
 * PRE: VALUE_MIN <= loop_count <= VALUE_DIGIT_MAX;
 *	    phrase occupies digital display at PHRASE_LENGTH ... DISPLAY_END.
 * POST: phrase occupies digital display at DISPLAY_START ... ((PHRASE_LENGTH - 1) - 1)
 *       and digital display at (DISPLAY_END - 1); creates a "broken" phrase effect.
 * RETURN: VOID
 */
void shiftBrokenPhrase(uint8_t loop_count) {
	/********************/
	/* Local Variables */
	/*******************/

	uint8_t case_index = FALSE;
	uint8_t phrase_index = FALSE;

	uint8_t display_position = FALSE;

	uint8_t exceeding_offset = FALSE;
	uint8_t remaining_phrase_displacement = FALSE;

	for (case_index = 0; case_index < BROKEN_CASES ; case_index++) {
		exceeding_offset = case_index + MAX_OFFSET;

		for (display_position = 0; display_position < DISPLAY_END; display_position++) {
			remaining_phrase_displacement = display_position - exceeding_offset;
			/* Part of Phrase Moved To Right End of Displays. */
			if (display_position <= case_index) {
				phrase_index = case_index - display_position;
				displayWrite(phrase[phrase_index], display_position);
			}
			/* Remaining Phrase at Left End of Displays. */
			else if (display_position > exceeding_offset) {
				phrase_index = display_position - (case_index + 2 * (remaining_phrase_displacement - 1));
				displayWrite(phrase[phrase_index], display_position);
			}

			/* Turn Off Digital Displays in Between Phrase. */
			refreshDisplay(case_index + 1, exceeding_offset + 1, FALSE);
			/* Loop Counter is Shown With a Single Digital Display Turned Off on Either Side. */
			writeCounter(loop_count, case_index + LAGGING_COUNT_OFFSET);
		}
		Sleep(DELAY_SHORT);
	}
}

/*
 * Print Error Message to Standard Output and Exit Program.
 * PARAM: message is a char array to print to screen.
 * PRE: error in running MATLAB engine or retrieving data.
 * POST: error message printed to stderr.
 * RETURN: VOID
 */
void handleError(char* message) {
	fprintf(stderr, message);
	system("PAUSE");

	exit(PROGRAM_ERROR);
}