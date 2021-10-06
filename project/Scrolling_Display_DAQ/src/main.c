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

static uint8_t phrase_message[PHRASE_LENGTH] = {
    DISPLAY_H,
	DISPLAY_E,
	DISPLAY_L,
	DISPLAY_L,
	DISPLAY_O
};

static uint8_t ending_message[ENDING_LENGTH] = {
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
	scrollDisplay(phrase_message, PHRASE_LENGTH, configDAQ());

	/* System Command Forces the System to Pause Before Closing Executable Window. */
	system("PAUSE");
	exit(TRUE);
}

int8_t configDAQ(void) {
	/* Local Variables */
	int display_config = FALSE;
	int number_scrolls = FALSE;

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
		handleError("\nExiting Program...\n\n");
	else if (sscanf_s(input_buff, "%d%s", (int*)&display_config, input_extra, BUFFSIZE) < TRUE)
		handleError("\nInvalid Input: Did Not Enter Integer...\n\n");
	else if (display_config != DEVICE_CONFIG && display_config != SIMULATOR_CONFIG)
		handleError("\nInvalid Display Configuration...\n\n");
	else setupDAQ(display_config);

	/* Scroll Limit Message */

	input_buff[0] = '0' + VALUE_DIGIT_MAX;
	strcpy_s(output_buff, BUFFSIZE, "Number of Iterations (Must Be Less Than or Equal To ");
	strncat_s(output_buff, BUFFSIZE, input_buff, sizeof(char));
	strcat_s(output_buff, BUFFSIZE, ") : ");

	/* Print to Standard Output. */
	fprintf(stdout, output_buff);

	if (!fgets(input_buff, BUFFSIZE, stdin))
		handleError("\nExiting Program...\n\n");
	else if (sscanf_s(input_buff, "%d%s", (int*)&number_scrolls, input_extra, BUFFSIZE) < TRUE)
		handleError("\nInvalid Input: Did Not Enter Integer...\n\n");
	else if ((number_scrolls < VALUE_MIN) || (number_scrolls > VALUE_DIGIT_MAX))
		handleError("\nInvalid Scrolling Configuration...\n\n");

	return number_scrolls;
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
void scrollDisplay(uint8_t* message, uint8_t message_length, int8_t number_scrolls) {
	uint8_t loop_count = FALSE;

	shiftStartingMessage(message, message_length);

	/* Implement Code For Scrolling Display. */
	while (++loop_count < number_scrolls) {
		refreshDisplay(message_length, NUMBER_DISPLAYS, FALSE);

		/* Cases with Intact Phrase Shifting Through Displays. */
		shiftCompleteMessage(message, message_length, loop_count);
		shiftSeparatedMessage(message, message_length, loop_count);
	}

	endDisplay(message, message_length, loop_count);
}

void shiftStartingMessage(uint8_t* message, uint8_t message_length) {
	/********************/
	/* Local Variables */
	/*******************/

	int8_t message_index = FALSE;

	int8_t relative_position = FALSE;
	int8_t display_position = FALSE;

	int8_t message_end = - message_length;

	/* Shift Message Until The Left Edge of the Digital Displays. */
	while (message_end <= VALUE_MIN) {
		/* Write Message to Digital Displays. */
		for (relative_position = 0; relative_position < message_length; relative_position++) {
			message_index = (message_length - 1) - relative_position;

			/* Shift Message Leftwards */
			display_position = relative_position + message_end;

			if (display_position >= VALUE_MIN) displayWrite(message[message_index], display_position);
		}

		/* Prepare to Shift Message. */
		message_end++;

		Sleep(DELAY_SHORT);
	}
}

/*
 * Phrase is Individually Shifted Off the Digital Displays.
 *
 * PARAM:     loop_count is a uint8_t representing the current loop iteration.
 * PRE:       0 <= loop_count <= VALUE_DIGIT_MAX;
 *            phrase occupies digital display at VALUE_MIN ... digital display at PHRASE_LENGTH.
 * POST:      all digital displays are turned off.
 * RETURN:    VOID
 */
void endDisplay(uint8_t* message, uint8_t message_length, uint8_t loop_count) {
	shiftCompleteMessage(message, message_length, loop_count);
	shiftFinishingMessage(message, message_length, loop_count);

	shiftStartingMessage(ending_message, ENDING_LENGTH);
	shiftCompleteMessage(ending_message, ENDING_LENGTH, FALSE);
	shiftFinishingMessage(ending_message, ENDING_LENGTH, FALSE);

	refreshDisplay(VALUE_MIN, NUMBER_DISPLAYS, TRUE);

	return;
}

/*
 * Turns On/Off Unused Digital Displays.
 *
 * PARAM: refresh_start and refresh_end are int8_ts which represent
 * 		  the left and right bounds of displays to be turned off respectively.
 * PRE:	VALUE_MIN <= refresh_start < refresh_end <= NUMBER_DISPLAYS
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

void shiftFinishingMessage(uint8_t* message, uint8_t message_length, uint8_t loop_count) {
	int8_t shift_index = FALSE;
	int8_t message_index = FALSE;

	int8_t relative_position = FALSE;
	int8_t display_position = FALSE;

	int8_t maximum_length = (loop_count != FALSE) ? message_length + LAGGING_COUNT_OFFSET : message_length;
	int8_t minimum_offset = NUMBER_DISPLAYS - maximum_length;

	int8_t message_end = FALSE;

	/* Individually Shift Message Off Digital Displays. */
	while (shift_index <= maximum_length) {
		message_end = shift_index + (NUMBER_DISPLAYS - message_length);

		for (display_position = (NUMBER_DISPLAYS - 1); display_position >= minimum_offset; display_position--) {
			relative_position = display_position - message_end;
			message_index = (message_length - 1) - relative_position;

			/* Write Message to the Appropriate Display Positions. */
			if (message_index < message_length) displayWrite(message[message_index], display_position);
			else displayWrite(DISPLAY_OFF, display_position);
		}

		if (loop_count != FALSE) {
			display_position = (NUMBER_DISPLAYS - maximum_length) + shift_index;
			if (display_position < NUMBER_DISPLAYS) writeCounter(loop_count, display_position);
		}

		shift_index++;

		Sleep(DELAY_SHORT);
	}
}

/*
 * Intact Phrase is Written to Appropriate Digital Displays to Create Scrolling Effect.
 *
 * PARAM: loop_count is an uint8_t representing the current loop iteration.
 * PRE: VALUE_MIN <= loop_count <= VALUE_DIGIT_MAX, 0 <= refresh_start <= refresh_end <= NUMBER_DISPLAYS
 * POST: phrase occupies digital display at PHRASE_LENGTH .. digital display at NUMBER_DISPLAYS.
 * RETURN: VOID
 */
void shiftCompleteMessage(uint8_t* message, uint8_t message_length, uint8_t loop_count) {
	/********************/
	/* Local Variables */
	/*******************/

	int8_t message_index = FALSE;

	int8_t relative_position = FALSE;
	int8_t display_position = FALSE;

	int8_t message_end = FALSE;

	int8_t maximum_offset = NUMBER_DISPLAYS - message_length;

	/* Shift Message Until It Occupies the Left End of the Digital Displays. */
	while (message_end <= maximum_offset) {
		/* Write Message to Digital Displays. */
		for (relative_position = 0; relative_position < message_length; relative_position++) {
			/* Turn Off all Displays Surrounding the Message. */
			refreshDisplay(VALUE_MIN, message_end, FALSE);
			refreshDisplay(message_end + message_length, NUMBER_DISPLAYS, FALSE);

			message_index = (message_length - 1) - relative_position;

			/* Shift Message Leftwards */
			display_position = relative_position + message_end;

			displayWrite(message[message_index], display_position);
		}

		/* Write Count to Appropriate Position. */

		if (loop_count != FALSE && message_end >= maximum_offset - 1) {
			display_position = message_end - LAGGING_COUNT_OFFSET;
			writeCounter(loop_count, display_position);
		}
		else if (loop_count > (VALUE_MIN + 1)) {
			display_position = message_end + message_length + LEADING_COUNT_OFFSET;
			writeCounter(loop_count - 1, display_position);
		}

		/* Prepare to Shift Message. */
		message_end++;

		Sleep(DELAY_SHORT);
	}
}


/*
 * Broken Phrase is Written to Appropriate Digital Displays to Create Scrolling Effect.
 *
 * PARAM: loop_count is a uint8_t representing the current loop iteration.
 * PRE: VALUE_MIN <= loop_count <= VALUE_DIGIT_MAX;
 *	    phrase occupies digital display at PHRASE_LENGTH ... NUMBER_DISPLAYS.
 * POST: phrase occupies digital display at VALUE_MIN ... ((PHRASE_LENGTH - 1) - 1)
 *       and digital display at (NUMBER_DISPLAYS - 1); creates a "broken" phrase effect.
 * RETURN: VOID
 */
void shiftSeparatedMessage(uint8_t* message, uint8_t message_length, uint8_t loop_count) {
	/********************/
	/* Local Variables */
	/*******************/

	int8_t message_index = FALSE;

	int8_t display_position = FALSE;

	int8_t message_start = FALSE;

	int8_t message_end = FALSE;
	int8_t relative_end = FALSE;

	for (message_start = VALUE_MIN; message_start < (message_length - 1); message_start++) {
		message_end = message_start + (NUMBER_DISPLAYS - message_length);

		for (display_position = 0; display_position < NUMBER_DISPLAYS; display_position++) {
			relative_end = display_position - message_end;
			/* Part of Message Moved To Right End of Displays. */
			if (display_position <= message_start) {
				message_index = message_start - display_position;
				displayWrite(message[message_index], display_position);
			}
			/* Remaining Message at Left End of Displays. */
			else if (display_position > message_end) {
				message_index = display_position - (message_start + 2 * (relative_end - 1));
				displayWrite(message[message_index], display_position);
			}

			/* Turn Off Digital Displays in Between Message. */
			refreshDisplay(message_start + 1, message_end + 1, FALSE);
			/* Loop Counter is Shown With a Single Digital Display Turned Off on Either Side. */
		}

		display_position = message_start + LAGGING_COUNT_OFFSET;
		writeCounter(loop_count, display_position);

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