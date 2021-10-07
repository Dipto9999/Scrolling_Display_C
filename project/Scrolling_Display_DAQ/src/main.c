/*
 * File : main.c
 * Purpose:	To Implement a Scrolling Message on 8-Segment Digital Displays.
 * 		Input: DAQ Module Configuration and Number of Scrolling Display Iterations.
 *		Output:	Message and Counter Value Scrolls on Digital Display Screen.
 * Author: Muntakim Rahman
 * Date: 2021-10-06
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

static uint8_t display_scrolls;

/*
 * Main Function Drives the Program. The Message Scrolls
 * on the Digital Displays Only if the DAQ Module is Appropriately Configured.
 * PRE: NULL (no pre-conditions)
 * POST: NULL (no side-effects)
 * RETURN: VOID
 */
void main(void) {
	display_scrolls = configDAQ();

	scrollDisplay(phrase_message, PHRASE_LENGTH, display_scrolls);

	/* Force System to Pause Before Closing Executable Window. */
	system("PAUSE");
	exit(TRUE);
}

/*
 * Prompt the User for DAQ Module Configuration and Number of Scrolling Display Configurations.
 * PRE: NULL (no pre-conditions)
 * POST: DAQ module is configured appropriately.
 * RETURN: number of iterations for scrolling display to complete.
 */
int8_t configDAQ(void) {
	/* Local Variables */

	int display_config = FALSE;
	int number_scrolls = FALSE;

	/* DAQ Configuration */
	strcpy_s(output_buff, BUFFSIZE, "Enter the Configuration Type: (");

	input_buff[0] = '0' + DEVICE_CONFIG;
	strncat_s(output_buff, BUFFSIZE, input_buff, sizeof(char));
	strcat_s(output_buff, BUFFSIZE, " for the Device, ");

	input_buff[0] = '0' + SIMULATOR_CONFIG;
	strncat_s(output_buff, BUFFSIZE, input_buff, sizeof(char));
	strcat_s(output_buff, BUFFSIZE, " for the Simulator): ");

	/* Print to Standard Output. */
	fprintf(stdout, output_buff);

	/* Check for Valid DAQ Configuration. */
	if (!fgets(input_buff, BUFFSIZE, stdin))
		handleError("\nExiting Program...\n\n");
	else if (sscanf_s(input_buff, "%d%s", (int*)&display_config, input_extra, BUFFSIZE) < TRUE)
		handleError("\nInvalid Input: Did Not Enter Integer...\n\n");
	else if (display_config != DEVICE_CONFIG && display_config != SIMULATOR_CONFIG)
		handleError("\nInvalid Display Configuration...\n\n");
	else setupDAQ(display_config);

	/* Number of Iterations */
	input_buff[0] = '0' + VALUE_COUNTER_MAX % (VALUE_DIGIT_MAX + 1);
	input_buff[1] = '0' + (VALUE_COUNTER_MAX / (VALUE_DIGIT_MAX + 1)) % (VALUE_DIGIT_MAX + 1);
	strcpy_s(output_buff, BUFFSIZE, "Number of Iterations (Must Be Less Than or Equal To ");
	strncat_s(output_buff, BUFFSIZE, input_buff, 2 * sizeof(char));
	strcat_s(output_buff, BUFFSIZE, ") : ");

	/* Print to Standard Output. */
	fprintf(stdout, output_buff);

	/* Check for Valid Scrolling Configuration. */
	if (!fgets(input_buff, BUFFSIZE, stdin))
		handleError("\nExiting Program...\n\n");
	else if (sscanf_s(input_buff, "%d%s", (int*)&number_scrolls, input_extra, BUFFSIZE) < TRUE)
		handleError("\nInvalid Input: Did Not Enter Integer...\n\n");
	else if ((number_scrolls < VALUE_MIN) || (number_scrolls > VALUE_COUNTER_MAX))
		handleError("\nInvalid Scrolling Configuration...\n\n");

	return number_scrolls;
}

/*
 * Scrolls a Message on the Digital Displays.
 *
 * PARAM: message is a uint8_t pointer to an array of bytes to write to the digital displays;
 * 		  message_length is a uint8_t which represents the number of bytes in the message;
 * 		  number_scrolls is an int8_t which represents the number of iterations;
 * PRE: DAQ module configured appropriately; VALUE_MIN <= number_scrolls <= VALUE_COUNTER_MAX.
 * POST: message is iteratively scrolled on the digital displays.
 * RETURN: VOID
 */
void scrollDisplay(uint8_t* message, uint8_t message_length, int8_t number_scrolls) {
	uint8_t counter = FALSE;

	configSwitches();

	/* Turn Off All Digital Displays. */
	refreshDisplay(VALUE_MIN, NUMBER_DISPLAYS, FALSE);

	shiftStartingMessage(message, message_length);

	/* Iteratively Scroll Message on Digital Displays. */
	while (++counter < number_scrolls) {
		shiftCompleteMessage(message, message_length, counter);
		shiftSeparatedMessage(message, message_length, counter);
	}

	/* Conclude Scrolling Display Iterations. */
	endDisplay(message, message_length, counter);
}

/*
 * Concludes Scrolling Display Iterations and Scrolls an Ending Message.
 *
 * PARAM: message is a uint8_t pointer to an array of bytes to write to the digital displays;
 * 		  message_length is a uint8_t which represents the number of bytes in the message;
 * 		  counter is a uint8_t representing the current iteration of the scrolling display.
 * PRE: message is written to appropriate digital displays; VALUE_MIN <= counter <= VALUE_COUNTER_MAX.
 * POST: digital displays are all turned on to indicate end of program.
 * RETURN: VOID
 */
void endDisplay(uint8_t* message, uint8_t message_length, uint8_t counter) {
	/* Shift Message Off of Digital Displays. */
	shiftCompleteMessage(message, message_length, counter);
	shiftFinishingMessage(message, message_length, counter);

	/* Shift Ending Message to Indicate End of Program. */
	shiftStartingMessage(ending_message, ENDING_LENGTH);
	shiftCompleteMessage(ending_message, ENDING_LENGTH, FALSE);
	shiftFinishingMessage(ending_message, ENDING_LENGTH, FALSE);

	/* Turn All Digital Displays On. */
	refreshDisplay(VALUE_MIN, NUMBER_DISPLAYS, TRUE);

	Sleep(DELAY_STANDARD);
	exit(TRUE);
}

/*
 * Message is Scrolled Onto the Digital Displays.
 *
 * PARAM: message is a uint8_t pointer to an array of bytes to write to the digital displays;
 * 		  message_length is a uint8_t which represents the number of bytes in the message;
 * PRE: all digital displays are turned off.
 * POST: message is written to appropriate digital displays;
 * RETURN: VOID
 */
void shiftStartingMessage(uint8_t* message, uint8_t message_length) {
	/* Local Variables */

	int8_t message_index = FALSE;

	int8_t relative_position = FALSE;
	int8_t display_position = FALSE;

	int8_t message_end = - (message_length - 1);

	/* Shift Message Onto the Digital Displays. */
	while (message_end <= VALUE_MIN) {
		if (resetDAQ() == TRUE) scrollDisplay(message, message_length, display_scrolls);

		/* Write Message to Digital Displays. */
		for (relative_position = 0; relative_position < message_length; relative_position++) {
			message_index = (message_length - 1) - relative_position;

			/* Shift Message Leftwards. */
			display_position = relative_position + message_end;

			/* Write Message to Display Positions Within Specified Range. */
			if (display_position >= VALUE_MIN) displayWrite(message[message_index], display_position);
		}

		/* Prepare to Shift Message. */
		message_end++;

		Sleep(DELAY_SHORT);
	}
}

/*
 * Message is Scrolled Off the Digital Displays.
 *
 * PARAM: message is a uint8_t pointer to an array of bytes to write to the digital displays;
 * 		  message_length is a uint8_t which represents the number of bytes in the message;
 * 		  counter is a uint8_t representing the current iteration of the scrolling display.
 * PRE: message is written to appropriate digital displays; VALUE_MIN <= counter <= VALUE_COUNTER_MAX.
 * POST: all digital displays are turned off to indicate end of scrolling iterations.
 */
void shiftFinishingMessage(uint8_t* message, uint8_t message_length, uint8_t counter) {
	/* Local Variables */

	int8_t shift_index = FALSE;
	int8_t message_index = FALSE;

	int8_t relative_position = FALSE;
	int8_t starting_position = FALSE;
	int8_t display_position = FALSE;

	int8_t maximum_length = FALSE;
	int8_t message_end = FALSE;

	if (counter == FALSE) maximum_length = message_length;
	else if (counter <= VALUE_DIGIT_MAX) maximum_length = message_length + MIN_COUNTER_LENGTH;
	else if (counter <= VALUE_COUNTER_MAX) maximum_length = message_length + MAX_COUNTER_LENGTH;

	starting_position = NUMBER_DISPLAYS - maximum_length;

	/* Shift Message Off of the Digital Displays. */
	while (shift_index <= maximum_length) {
		if (resetDAQ() == TRUE) scrollDisplay(message, message_length, display_scrolls);

		message_end = shift_index + (NUMBER_DISPLAYS - message_length);

		for (display_position = (NUMBER_DISPLAYS - 1); display_position >= starting_position; display_position--) {
			relative_position = display_position - message_end;
			message_index = (message_length - 1) - relative_position;

			/* Write Message to the Appropriate Display Positions. */
			if (message_index < message_length) displayWrite(message[message_index], display_position);
			else displayWrite(DISPLAY_OFF, display_position);
		}

		/* Write Counter to Appropriate Display Position. */
		if (counter != FALSE) {
			display_position = (NUMBER_DISPLAYS - maximum_length) + shift_index;
			writeCounter(counter, display_position);
		}

		/* Prepare to Shift Message. */
		shift_index++;

		Sleep(DELAY_SHORT);
	}
}

/*
 * Complete Message is Shifted on Digital Displays.
 *
 * PARAM: message is a uint8_t pointer to an array of bytes to write to the digital displays;
 * 		  message_length is a uint8_t which represents the number of bytes in the message;
 * 		  counter is a uint8_t representing the current iteration of the scrolling display.
 * PRE: message is written to appropriate digital displays; VALUE_MIN <= counter <= VALUE_COUNTER_MAX.
 * POST: message is written to appropriate digital displays;
 * RETURN: VOID
 */
void shiftCompleteMessage(uint8_t* message, uint8_t message_length, uint8_t counter) {
	/* Local Variables */

	int8_t message_index = FALSE;

	int8_t relative_position = FALSE;
	int8_t display_position = FALSE;
	int8_t starting_position = NUMBER_DISPLAYS - message_length;

	int8_t message_end = FALSE;

	/* Shift Message Leftwards On the Digital Displays. */
	while (message_end <= starting_position) {
		if (resetDAQ() == TRUE) scrollDisplay(message, message_length, display_scrolls);

		for (relative_position = 0; relative_position < message_length; relative_position++) {
			message_index = (message_length - 1) - relative_position;
			display_position = relative_position + message_end;

			/* Write Message to the Appropriate Display Positions. */
			displayWrite(message[message_index], display_position);
		}

		/* Turn Off Digital Displays Surrounding the Message. */
		refreshDisplay(VALUE_MIN, message_end, FALSE);
		refreshDisplay(message_end + message_length, NUMBER_DISPLAYS, FALSE);

		/* Write Counter to Appropriate Display Position. */
		if (counter != FALSE && message_end >= starting_position - 1) {
			display_position = (counter > VALUE_DIGIT_MAX) ?
				message_end - MAX_COUNTER_LENGTH : message_end - MIN_COUNTER_LENGTH;
			writeCounter(counter, display_position);
		}
		else if (counter > (VALUE_MIN + 1)) {
			display_position = message_end + (message_length - 1) + LEADING_COUNTER_OFFSET;
			writeCounter(counter - 1, display_position);
		}

		/* Prepare to Shift Message. */
		message_end++;

		Sleep(DELAY_SHORT);
	}
}


/*
 * Broken Phrase is Written to Appropriate Digital Displays to Create Scrolling Effect.
 *
 * PARAM: counter is a uint8_t representing the current loop iteration.
 * PRE: VALUE_MIN <= counter <= VALUE_DIGIT_MAX;
 *	    phrase occupies digital display at PHRASE_LENGTH ... NUMBER_DISPLAYS.
 * POST: phrase occupies digital display at VALUE_MIN ... ((PHRASE_LENGTH - 1) - 1)
 *       and digital display at (NUMBER_DISPLAYS - 1); creates a "broken" phrase effect.
 * RETURN: VOID
 */

/*
 * Message is Separately Shifted on Digital Displays.
 *
 * PARAM: message is a uint8_t pointer to an array of bytes to write to the digital displays;
 * 		  message_length is a uint8_t which represents the number of bytes in the message;
 * 		  counter is a uint8_t representing the current iteration of the scrolling display.
 * PRE: message is written to appropriate digital displays; VALUE_MIN <= counter <= VALUE_COUNTER_MAX.
 * POST: message is written to appropriate digital displays;
 * RETURN: VOID
 */
void shiftSeparatedMessage(uint8_t* message, uint8_t message_length, uint8_t counter) {
	/* Local Variables */

	int8_t message_index = FALSE;

	int8_t display_position = FALSE;
	int8_t relative_position = FALSE;

	int8_t message_start = FALSE;
	int8_t message_end = FALSE;

	message_start = (counter > VALUE_DIGIT_MAX) ? VALUE_MIN - 1 : VALUE_MIN;

	/* Shift Message Leftwards On the Digital Displays. */
	while (message_start < (message_length - 1)) {
		if (resetDAQ() == TRUE) scrollDisplay(message, message_length, display_scrolls);

		message_end = (counter > VALUE_DIGIT_MAX) ?
			(message_start + 1) + (NUMBER_DISPLAYS - message_length) : message_start + (NUMBER_DISPLAYS - message_length);

		for (display_position = 0; display_position < NUMBER_DISPLAYS; display_position++) {
			relative_position = display_position - message_end;
			/* Part of Message Moved To Right End of Displays. */
			if (display_position <= message_start) {
				message_index = message_start - display_position;
				/* Write Message to the Appropriate Display Positions. */
				displayWrite(message[message_index], display_position);
			}
			/* Remaining Message at Left End of Displays. */
			else if (display_position > message_end) {
				message_index = (counter > VALUE_DIGIT_MAX) ?
					display_position - ((message_start + 1) + 2 * (relative_position - 1)) : display_position - (message_start + 2 * (relative_position - 1));
				/* Write Message to the Appropriate Display Positions. */
				displayWrite(message[message_index], display_position);
			}
		}

		display_position = message_start + LEADING_COUNTER_OFFSET;

		/* Write Counter to Appropriate Display Position. */
		refreshDisplay(message_start + 1, message_end + 1, FALSE);
		writeCounter(counter, display_position);

		/* Prepare to Shift Message. */
		message_start++;

		Sleep(DELAY_SHORT);
		}
}

/*
 * Turns On/Off Specified Digital Displays.
 *
 * PARAM: refresh_start and refresh_end are uint8_ts which represent
 * 		  the left and right bounds of displays to be turned off respectively.
 * PRE:	VALUE_MIN <= refresh_start < refresh_end < NUMBER_DISPLAYS
 * POST: digital display at refresh_start ... digital display at refresh_end
 * 		 are all either turned on or off.
 * RETURN: VOID
 */
void refreshDisplay(uint8_t refresh_start, uint8_t refresh_end, uint8_t turn_on) {
	for (uint8_t refresh_index = refresh_start; refresh_index < refresh_end; refresh_index++) {
		if (turn_on == FALSE) displayWrite(DISPLAY_OFF, refresh_index);
		else if (turn_on == TRUE) displayWrite(DISPLAY_ON, refresh_index);
	}
}

/*
 * Print Error Message to Standard Output Terminal and Exit Program.
 * PARAM: message is a pointer to a char array to print to screen.
 * PRE: error in scrolling display program configuration.
 * POST: error message printed to stderr.
 * RETURN: VOID
 */
void handleError(char* message) {
	fprintf(stderr, message);
	system("PAUSE");

	exit(PROGRAM_ERROR);
}