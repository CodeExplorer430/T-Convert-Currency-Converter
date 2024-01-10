// user_interaction.h - Header file for user interaction functions

#ifndef USER_INTERACTION_H
#define USER_INTERACTION_H


// Validates the user input for an amount and returns the validated amount
double validateAmount();

// Gets the user's choice from input with error handling, updating the passed pointer to the choice value
int getChoiceWithErrorHandling(int *choice);

// Gets the user's primary choice
int getUserChoice();

// Validates the sub-choice within a specified range and updates the passed pointer to the choice value
int validateSubChoice(int *choice);

// Handles the user's exit confirmation choice and returns the selected action
int confirmExitChoice();

#endif /* USER_INTERACTION_H */
