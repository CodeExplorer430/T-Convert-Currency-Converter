// date_utils.c - Source file for date utility functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include <stdbool.h>    // Library for using boolean data type with true and false values
#include <string.h>     // Library for string manipulation functions like strlen, strcpy, etc.
#include <ctype.h>      // Library for character handling functions like isdigit, isalpha, etc.
#include <time.h>       // Library for date and time functions like time, localtime, etc.
#include "date_utils.h" // Header file containing utility functions for handling dates and times


// Function to validate the date input
void validateDateInput(char* date) {
    while (true) {
        printf("\n\t\t\t\t\t\t\tEnter the date for exchange rates (YYYY-MM-DD or type 'today' for current date): \n");
        printf("\t\t\t\t\t\t\t> ");

        if (scanf("%10s", date) != 1) { // Read input and ensure it's not more than 10 characters
            printf("\n\t\t\t\t\t\t\tError: Invalid input.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        // Convert the user input to lowercase for case-insensitive comparison
        toLowercase(date);

        // Check if the user input is 'today' and fetch the current date
        if (strcmp(date, "today") == 0) {
            // Handle fetching current date here if needed
            getCurrentDate(date); // Get the current date in YYYY-MM-DD format
            printf("\n\n\t\t\t\t\t\t\tFetching current date...\n");
            break; // Break the loop for 'today' input
        }

        // Validate the entered date format
        if (validateDateFormat(date)) {
            break; // Valid date format, exit the loop
        } else {
            printf("\n\t\t\t\t\t\t\tError: Invalid date format. Please use YYYY-MM-DD format or type 'today'.\n");
        }
    }
}


// Function to validate date format (YYYY-MM-DD)
bool validateDateFormat(const char* date) {
    // Check if the input string's length is not equal to 10 (YYYY-MM-DD has 10 characters)
    if (strlen(date) != 10) {
        return false; // Return false for an invalid length
    }

    // Iterate through each character in the input string
    for (int i = 0; i < 10; ++i) {
        // Check for positions where the delimiter '-' should be present (positions 4 and 7)
        if (i == 4 || i == 7) {
            // If the character is not a '-', return false for an invalid delimiter
            if (date[i] != '-') {
                return false; // Return false for an invalid delimiter
            }
        } else {
            // Check if the characters at positions other than the delimiter are not digits
            if (date[i] < '0' || date[i] > '9') {
                return false; // Return false for an invalid character (non-digit)
            }
        }
    }
    return true; // If all conditions pass, return true for a valid format
}


// Function to retrieve and display the current time in a formatted string
void displayLastUpdatedTime() {
    time_t rawtime;          // Variable to hold raw time data
    struct tm* timeinfo;     // Struct to store formatted time data
    char buffer[80];         // Buffer to hold formatted time string

    time(&rawtime);          // Get current time in raw format
    timeinfo = localtime(&rawtime); // Convert raw time to local time info

    // Format time as per the specified format and store in 'buffer'
    strftime(buffer, sizeof(buffer), "Last updated: %Y-%m-%d %H:%M %Z", timeinfo);

    // Display the formatted time
    printf("\n\n\t\t\t\t\t\t\t%s\n", buffer);
}


// Function to retrieve the current date and format it as YYYY-MM-DD
// Parameters:
// - currentDate: Pointer to a character array to store the formatted date
void getCurrentDate(char* currentDate) {
    time_t t = time(NULL);             // Get current time in seconds
    struct tm tm = *localtime(&t);     // Convert to local time structure

    // Format the current date as YYYY-MM-DD and store it in 'currentDate'
    sprintf(currentDate, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}


// Function to get the current date in YYYY-MM-DD format (UTC)
void getCurrentDateUTC(char* currentDate) {
    time_t rawtime;         // Variable to hold the raw time (seconds since epoch)
    struct tm* timeinfo;    // Structure to hold time components

    time(&rawtime);         // Get current time in seconds since epoch

    timeinfo = gmtime(&rawtime); // Convert raw time to UTC time structure

    // Format the time according to YYYY-MM-DD and store it in 'currentDate'
    // strftime() formats time according to the specified format and stores it in the provided string
    strftime(currentDate, 11, "%Y-%m-%d", timeinfo);
    // '%Y' - Year with century as a decimal number
    // '%m' - Month as a decimal number (01-12)
    // '%d' - Day of the month as a decimal number (01-31)
}


// Function to convert the input string to lowercase
// Parameters:
// - str: Pointer to the string to be converted to lowercase
void toLowercase(char* str) {
    while (*str) {
        *str = tolower((unsigned char)*str); // Convert each character to lowercase
        str++;                               // Move to the next character
    }
}

