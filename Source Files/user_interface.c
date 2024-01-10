// user_interface.c - Source file for user interface functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include "user_interface.h" // Header file for managing user interface functions


// Function to display the initial interface
void displayInitialInterface() {
    // Clear the console screen (system-specific: "cls" for Windows, "clear" for Unix)
    system("cls"); // Use "clear" for Unix
    
    // Set console color (for Windows)
    system("COLOR 0B");
    
    // Display ASCII art and interface
    printf("                                           /$$$$$$$$       /$$$$$$                                                      /$$    \n");
    printf("                                          |__  $$__/      /$$__  $$                                                    | $$    \n");
    printf("                                             | $$        | $$  \\__/  /$$$$$$  /$$$$$$$  /$$    /$$ /$$$$$$   /$$$$$$  /$$$$$$  \n");
    printf("                                             | $$ /$$$$$$| $$       /$$__  $$| $$__  $$|  $$  /$$//$$__  $$ /$$__  $$|_  $$_/  \n");
    printf("                                             | $$|______/| $$      | $$  \\ $$| $$  \\ $$ \\  $$/$$/| $$$$$$$$| $$  \\__/  | $$    \n");
    printf("                                             | $$        | $$    $$| $$  | $$| $$  | $$  \\  $$$/ | $$_____/| $$        | $$ /$$\n");
    printf("                                             | $$        |  $$$$$$/|  $$$$$$/| $$  | $$   \\  $/  |  $$$$$$$| $$        |  $$$$/\n");
    printf("                                             |__/         \\______/  \\______/ |__/  |__/    \\_/    \\_______/|__/         \\___/  \n");
    printf("\n\n\n");

    printf("\n\n\t\t\t\t\t\t+========================================================================+");
    printf("\n\t\t\t\t\t\t|-------------------T-Convert: Currency Converter v1.0-------------------|\t\t\t\t\n");
    printf("\t\t\t\t\t\t+========================================================================+");
    printf("\n\n\t\t\t\t\t\t\t\t\t   Welcome to T-Convert!\n");
    printf("\t\t\t\t\t\t\t\t\t     Version 1.0 2023\n");
    printf("\n\t\t\t\t\t\t\t---------------------------------------------------------\n\n\n");

    printf("\t\t\t\t\t\t\tPlease select an option: \n");

    printf("\t\t\t\t\t\t\t1. Convert Currency\n");
    printf("\t\t\t\t\t\t\t2. View Supported Currencies\n");
    printf("\t\t\t\t\t\t\t3. Exit\n\n");
}

