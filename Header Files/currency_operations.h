// currency_operations.h - Header file for currency operation functions

#ifndef CURRENCY_OPERATIONS_H
#define CURRENCY_OPERATIONS_H

// Function to perform currency conversion
double convertCurrency(double amount, double exchangeRate);

// Function to fetch supported currencies
void fetchSupportedCurrencies();

// Function to fetch and display supported currencies
void displaySupportedCurrencies();

// Function to perform currency conversion
void performCurrencyConversion(double amount, const char* fromCurrency, const char* toCurrency, const char* date);

#endif /* CURRENCY_OPERATIONS_H */
