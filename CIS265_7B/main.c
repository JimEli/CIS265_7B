/*************************************************************************
* Title: (CIS265_7A) Date Validity Checker & Formatter.
* File: main.c
* Author: James Eli
* Date: 9/1/2017
*
* Write a program that accepts a date from the user in the form 
* mm/dd/yyyy and then displays it in the form month dd, yyyy, where 
* month is the name of the month:
*   Enter a date (m/dd/yyyy): 2/17/2011
*   You entered the date February 17, 2011
* Store the month names in an array that contains pointers to string.
*
* Notes:
*  (1) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*  (2) Performs validity check on date entered.
*  (3) Uses secure version of sscanf.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   09/01/2017: Initial release. JME
*   09/17/2017: Corected logic error determining year validation. JME
*************************************************************************/
#include <assert.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definitions.
#define MAX_INPUT_ATTEMPTS 3    // Maximum input attempts before aborting.
#define STRING_LENGTH      20   // Length of string holding user input.
#define MIN_YEAR           1583 // Start of Gregorian calendar.
#define MAX_YEAR           4000 // End date.

// Simple date structure.
struct  Date_ {
	int year;
	unsigned int month;
	unsigned int day;
};
typedef struct Date_ Date;

// Leap year validator.
// Logic: if year is divisible by 4, then its a leap year.
// But if that year is divisible by 100, then its not a leap year.
// However, if the year is also divisible by 400, then its a leap year.
inline bool isLeapYear(const unsigned int year) {
	return ((!(year % 4) && (year % 100)) || !(year % 400));
}

// Calculate last day of given year/month, taking leap year into account.
unsigned int lastDayOfMonth(const int y, const unsigned int m) {
	unsigned int eom[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	assert(m >= 1 && m <= 12);
	return m != 2 || !isLeapYear(y) ? eom[m] : 29U;
}

// Return true if day is within proper range for month & year.
inline bool validateDay(const int y, const unsigned int m, const unsigned int d) {
	return(d >= 1 && d <= lastDayOfMonth(y, m));
}

// Return true if month is within range 1-12.
inline bool validateMonth(const unsigned int m) {
	return (m >= 1 && m <= 12);
}

// Return true if year is within range.
inline bool validateYear(const unsigned int y) {
	return (y >= MIN_YEAR && y <= MAX_YEAR);
}

/**********************************************************************
 * Return true if user enters a valid date in the proper format of 
 * "mm/dd/yyyy". The date entered is checked for proper month, year and 
 * day bounds (including leap year).
 *********************************************************************/
bool getDate(Date *date) {
	char s[STRING_LENGTH];             // String holds temporary user input.
	int attempts = MAX_INPUT_ATTEMPTS; // Input attempt counter.
	bool retVal = false;               // Return value from function, true == success.

	assert(attempts > 0); // Assert attempts valid non-zero, positive number.

	// Attempt only so many inputs.
	while (attempts--) {
		// Prompt and grab user input.
		fputs("Enter a date in the following format \"mm/dd/yyyy\": ", stdout);
		if (!fgets(s, STRING_LENGTH, stdin)) {
			fputs("\nFatal program error!\n", stderr);
			exit(EXIT_FAILURE);
		}
		else if (!strchr(s, '\n')) {
			// input too long, eat remainder.
			while (fgets(s, STRING_LENGTH, stdin) && !strchr(s, '\n'))
				; // Empty body.
			fputs("Too many characters input.\n", stdout);
		}
		else {
			// Attempt to convert from string to float, and validate.
			if (sscanf_s(s, "%2d/%2d/%4d", &date->month, &date->day, &date->year)) {
				if (!validateYear(date->year))
					fputs("Invalid year enetered.\n", stdout);
				else if (!validateMonth(date->month))
					fputs("Invalid month enetered.\n", stdout);
				else if (date->day >= 1 && date->day <= lastDayOfMonth(date->year, date->month)) {
					// Passed all checks, susccess! Return.
					retVal = true;
					break;
				}
				else
					fputs("Invalid day enetered.\n", stdout);
			}
		}
	}
	return retVal;
}

// Program starts here.
int main(void) {
	const char *months[12] = { 
		"January", "February", "Mrach", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};

	Date date;
	// If input is acceptable, display date in new format.
	if (getDate(&date))
		fprintf(stdout, "You entered the date %s %d, %4d\n\n", months[date.month - 1], date.day, date.year);
	else
		fputs("Terminating program.\n", stdout);
}
