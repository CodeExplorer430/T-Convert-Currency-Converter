// utilities.c - Source file for utility functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include "utilities.h" // Header file for miscellaneous utility functions


// Function to clear the input buffer
void clearInputBuffer() {
    int c;
    // Loop through and consume characters in the input buffer until encountering a newline or EOF
    while ((c = getchar()) != '\n' && c != EOF);
    // Note: EOF represents the end-of-file indicator
}

