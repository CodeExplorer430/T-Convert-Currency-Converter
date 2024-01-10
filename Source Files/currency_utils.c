// currency_utils.c - Source file for currency utility functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include <stdbool.h>    // Library for using boolean data type with true and false values
#include <string.h>     // Library for string manipulation functions like strlen, strcpy, etc.
#include "api_utils.h"  // Header file containing API-related constants, structures, and functions
#include "currency_utils.h" // Header file providing utility functions for currency handling


// Function to validate a currency code
// Checks if the provided currency code exists in the list of supported currencies
bool isValidCurrency(const char* currencyCode) {
    // Loop through the supportedCurrencies array to check if the inputted currency code is valid
    for (int i = 0; i < numberOfCurrencies; ++i) {
        if (strcmp(currencyCode, supportedCurrencies[i]) == 0) {
            return true; // Valid currency code found
        }
    }
    return false; // Currency code not found in the supported list
}


// Function to validate user input for a currency code
// Prompts the user to input a currency code for a specified type (source or target)
// Loops until a valid non-empty currency code is entered, displaying error messages as needed
void validateCurrency(char* currency, const char* type) {
    while (1) {
        printf("\n\t\t\t\t\t\t\tPlease select your %s currency (e.g., GBP, USD, EUR): \n", type);
        printf("\t\t\t\t\t\t\t> ");
        fgets(currency, 50, stdin); // Read input

        // Remove newline character if present
        size_t length = strlen(currency);
        if (length > 0 && currency[length - 1] == '\n') {
            currency[length - 1] = '\0';
        }

        // Validate empty input
        if (strlen(currency) == 0) {
            printf("\n\t\t\t\t\t\t\tError: Please enter a valid %s currency code.\n", type);
            continue; // Continue loop to prompt for input again
        }

        // Validate currency code
        if (!isValidCurrency(currency)) {
            fprintf(stderr, "\n\t\t\t\t\t\t\tError: Invalid %s currency code.\n", type);
        } else {
            break; // Valid currency code entered, exit the loop
        }
    }
}
