// date_utils.h - Header file for date utility functions

#ifndef DATE_UTILS_H
#define DATE_UTILS_H


// Validates the user input for date
void validateDateInput(char* date);

// Validates the format of the date (e.g., YYYY-MM-DD)
bool validateDateFormat(const char* date);

// Displays the last updated time
void displayLastUpdatedTime();

// Retrieves the current date in the local time zone
void getCurrentDate(char* currentDate);

// Retrieves the current date in UTC format
void getCurrentDateUTC(char* currentDate);

// Converts a string to lowercase
void toLowercase(char* str);


#endif /* DATE_UTILS_H */
