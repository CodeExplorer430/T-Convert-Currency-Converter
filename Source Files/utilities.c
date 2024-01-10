// utilities.c - Source file for utility functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include "utilities.h" // Header file for miscellaneous utility functions


// Function to clear the input buffer
void clearInputBuffer() {
    int character;
    // Loop through and consume characters in the input buffer until encountering a newline or EOF
    while ((character = getchar()) != '\n' && character != EOF) {
        // Do nothing, just consume characters
        // Note: EOF represents the end-of-file indicator
    }
    // Clear any remaining characters in case of an error or unexpected situation
    if (character == EOF && ferror(stdin)) {
        clearerr(stdin);
    }
}
