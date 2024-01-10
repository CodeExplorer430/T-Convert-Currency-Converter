// user_interaction.c - Source file for user interaction functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include <string.h>     // Library for string manipulation functions like strlen, strcpy, etc.
#include <ctype.h>      // Library for character handling functions like isdigit, isalpha, etc.
#include "user_interaction.h" // Header file for user interaction functionalities


// Function to validate the entered amount
// Returns a double value representing the validated amount
double validateAmount() {
    double amount;
    char input[50];
    int isValid = 0;

    while (!isValid) {
        printf("\n\t\t\t\t\t\t\tEnter the amount to convert: ");
        printf("\n\t\t\t\t\t\t\t> ");
        fgets(input, sizeof(input), stdin); // Read input

        // Remove newline character if present
        size_t length = strlen(input);
        if (input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        // Check for empty input
        if (strlen(input) == 0) {
            printf("\n\t\t\t\t\t\t\tError: Please enter a valid amount.\n");
            continue; // Continue loop to prompt for input again
        }

        // Check if the input is '0'
        if (strlen(input) == 1 && input[0] == '0') {
            printf("\n\t\t\t\t\t\t\tError: Please enter a non-zero amount.\n");
            continue; // Continue loop to prompt for input again
        }

        // Check if input contains only valid characters (digits and a single dot)
        isValid = 1;
        for (int i = 0; input[i] != '\0'; ++i) {
            if (!isdigit(input[i]) && input[i] != '.') {
                isValid = 0;
                break;
            }
        }

        // If the input is valid, convert it to a double
        if (isValid) {
            amount = strtod(input, NULL);

            // Check if the amount is positive
            if (amount > 0) {
                return amount;
            } else {
                printf("\n\t\t\t\t\t\t\tError: Please enter a positive amount.\n");
                isValid = 0; // Reset isValid to re-prompt for input
            }
        } else {
            printf("\n\t\t\t\t\t\t\tError: Invalid input. Please enter a valid amount.\n");
        }
    }
    return 0; // Default return value (not expected to reach this point)
}


// Function to get user input and validate for a choice between 1 and 3
int getChoiceWithErrorHandling(int *choice) {
    char input[50]; // Array to store user input
    
    fgets(input, sizeof(input), stdin); // Read user input

    // Remove newline character if present
    size_t length = strlen(input);
    if (input[length - 1] == '\n') {
        input[length - 1] = '\0';
    }

    // Check if the input consists of valid digits
    int isValid = 1;
    for (int i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            isValid = 0;
            break;
        }
    }

    // If the input is not valid or empty, display an error message
    if (!isValid || strlen(input) == 0) {
        printf("\n\t\t\t\t\t\t\tInvalid input. Please enter a valid choice (1-3).\n\n");
        printf("\n");
        return 0; // Return 0 to indicate invalid input
    }

    // Convert input to integer and check if it's within the valid range (1-3)
    *choice = atoi(input);
    if (*choice < 1 || *choice > 3) {
        printf("\n\t\t\t\t\t\t\tInvalid input. Please enter a valid choice (1-3).\n\n");
        printf("\n");
        return 0; // Return 0 to indicate invalid input
    }

    return 1; // Return 1 to indicate valid input
}


// Function to get the user's choice with error handling
int getUserChoice() {
    int choice = 0; // Initialize the choice variable

    // Loop indefinitely until a valid choice is entered
    while (1) {
        printf("\t\t\t\t\t\t\tEnter your choice (1-3): \n");
        printf("\t\t\t\t\t\t\t> ");

        // Get user input with error handling to ensure it's a number between 1 and 3
        int inputSuccess = getChoiceWithErrorHandling(&choice);

        // If input is unsuccessful (invalid choice), continue prompting for input
        if (!inputSuccess) {
            continue;
        } else {
            return choice; // Exit the loop and return the valid choice
        }
    }
}


// Function to validate and get the sub-choice for post-conversion options
int validateSubChoice(int *choice) {
    int subChoice; // Variable to store sub-choice

    do {
        // Display options after conversion
        printf("\n\n\t\t\t\t\t\t\tOptions");
        printf("\n\n\t\t\t\t\t\t\t1. Perform another conversion\n");
        printf("\t\t\t\t\t\t\t2. Back to the main menu\n");
        printf("\t\t\t\t\t\t\t3. Exit\n\n");

        printf("\t\t\t\t\t\t\tEnter your choice (1-3): \n");
        printf("\t\t\t\t\t\t\t> ");

        int inputSuccess = getChoiceWithErrorHandling(&subChoice);

        if (!inputSuccess) {
            continue; // Loop if invalid input
        } else if (subChoice == 2) {
            printf("\n\n\t\t\t\t\t\t\tReturning to the main menu...");
            *choice = subChoice; // Set the value of choice to the validated sub-choice        
            return 1; // Exit loop if a valid choice is entered
        } else {
            *choice = subChoice; // Set the value of choice to the validated sub-choice        
            return 1; // Exit loop if a valid choice is entered
        }
    } while (1);

    return 0; // Default return value (not expected to reach this point)
}


// Function to handle exit confirmation
int confirmExitChoice() {
    char input[50]; // Buffer to store user input
    int validInput; // Flag to check if the input is valid
    int choice = 0; // Default choice (0 - not exiting)

    do {
        // Exit confirmation prompt
        printf("\n\n\t\t\t\t\t\t\tAre you sure you want to exit? (Y/N): \n");
        printf("\t\t\t\t\t\t\t> ");
        fgets(input, sizeof(input), stdin); // Read input

        // Remove newline character if present
        size_t length = strlen(input);
        if (input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        validInput = 1; // Assuming input is valid until proven otherwise

        if (strlen(input) == 0) {
            printf("\n\t\t\t\t\t\t\tInvalid input. Please enter 'Y' or 'N'.\n");
            validInput = 0; // Invalid input, continue loop to prompt again
        } else if (input[0] == 'Y' || input[0] == 'y') {
            printf("\n\n\t\t\t\t\t\t**********************THANK YOU FOR USING T-CONVERT!**********************");
            printf("\n\t\t\t\t\t\t^^^^^^^^^^^^^[Developed by the group of Velasco from BSIT 1B]^^^^^^^^^^^^^");
            printf("\n\t\t\t\t\t\t+========================================================================+\n");
            choice = 3; // Set choice to 3 (exit)
        } else if (input[0] == 'N' || input[0] == 'n') {
            printf("\n\n\t\t\t\t\t\t\tReturning to the main menu...");
            choice = 0; // Reset choice to return to the main menu
        } else {
            printf("\n\t\t\t\t\t\t\tInvalid choice. Please enter 'Y' or 'N'.\n");
            validInput = 0; // Invalid input, continue loop to prompt again
        }
    } while (!validInput);

    return choice; // Return the chosen action (exit or return to main menu)
}

