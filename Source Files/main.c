#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include <stdbool.h>    // Library for using boolean data type with true and false values
#include <string.h>     // Library for string manipulation functions like strlen, strcpy, etc.
#include <conio.h>      // Header file for console I/O functions (mainly for getch() to read character input without echoing it)
#include <ctype.h>      // Library for character handling functions like isdigit, isalpha, etc.
#include <time.h>       // Library for date and time functions like time, localtime, etc.
#include <winsock2.h>  // Header providing Winsock 2 API declarations for network programming on Windows
#include <windows.h>    // Library providing functions for Windows API and system-related functions
#include <curl/curl.h>  // Library for making HTTP requests and working with URLs using libcurl
#include "cJSON.h"      // Header for cJSON, a lightweight JSON parsing library
#include "currency_operations.h" // Header file for currency operations functionality
#include "currency_utils.h" // Header file providing utility functions for currency handling
#include "user_interface.h" // Header file for managing user interface functions
#include "user_interaction.h" // Header file for user interaction functionalities
#include "date_utils.h" // Header file containing utility functions for handling dates and times


int main() {
    // Variables to store user inputs and choice
    double amount;
    char fromCurrency[10], toCurrency[10], date[11];
    char currentDate[11];
    int choice = 0;

    // Main loop controlling the menu
    while (choice != 3) {
        // Fetch supported currencies from the API
        fetchSupportedCurrencies();

        // Display initial interface
        displayInitialInterface();

        // Get user's choice from the menu
        choice = getUserChoice();
									
        switch (choice) {
            case 1:
                do {
                    printf("\n\n\t\t\t\t\t\t\tConvert Currency");
                    printf("\n\t\t\t\t\t\t\t=================================================================\n");

                    // Get source and target currencies and validate
                    validateCurrency(fromCurrency, "source");
                    validateCurrency(toCurrency, "target");

                    // Validate and get the amount to convert
                    amount = validateAmount();

                    // Validate and get the date for exchange rates
                    validateDateInput(date);

                    printf("\n\n\t\t\t\t\t\t\tFetching latest exchange rates...\n\n");

                    // Perform currency conversion using FX Rates API
                    performCurrencyConversion(amount, fromCurrency, toCurrency, date);

                    int subChoice;

                    // Validate and process the user's sub-choice for this menu
                    while (!validateSubChoice(&subChoice));

                    if (subChoice == 2) {
                        // Return to the main menu
                        printf("\n");
                        choice = 0; // Reset choice to display the initial interface again
                        break; // Exit the 'do-while' loop for case 1
                    } else if (subChoice == 3) {
                        choice = confirmExitChoice(); // Confirm the exit choice
                        break; // Exit the 'do-while' loop for case 1
                    }
                    // Continue the loop for case 1 for another conversion
                } while (choice == 1);
                break;

            case 2:
                printf("\n\n\t\t\t\t\t\t\tSupported Currencies");
                printf("\n\t\t\t\t\t\t\t=================================================================");

                // Display supported currencies
                displaySupportedCurrencies();

                printf("\n\t\t\t\t\t\t\t=================================================================");

                printf("\n\n\t\t\t\t\t\t\tPress any key to return to the main menu...\n");
                getch(); // Wait for a key press
                printf("\n");

                break;

            case 3:
                // Confirm exit choice
                choice = confirmExitChoice();
                break;

            default:
                printf("\n\t\t\t\t\t\t\tInvalid choice. Please select again.\n");
        }
    } // End of while loop

    return 0;
}
