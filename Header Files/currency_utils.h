// currency_utils.h - Header file for currency utility functions

#ifndef CURRENCY_UTILS_H
#define CURRENCY_UTILS_H

#include <stdbool.h>


// Checks if a currency code is valid
bool isValidCurrency(const char* currencyCode);

// Validates the currency code based on a specified type
void validateCurrency(char* currency, const char* type);

#endif /* CURRENCY_UTILS_H */
