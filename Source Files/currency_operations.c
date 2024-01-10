// currency_operations.c - Source file for currency operation functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include <string.h>     // Library for string manipulation functions like strlen, strcpy, etc.
#include <winsock2.h>  // Header providing Winsock 2 API declarations for network programming on Windows
#include <windows.h>    // Library providing functions for Windows API and system-related functions
#include <curl/curl.h>  // Library for making HTTP requests and working with URLs using libcurl
#include "cJSON.h"      // Header for cJSON, a lightweight JSON parsing library
#include "currency_operations.h" // Header file for currency operations functionality
#include "currency_utils.h" // Header file providing utility functions for currency handling
#include "date_utils.h" // Header file containing utility functions for handling dates and times
#include "api_utils.h"  // Header file containing API-related constants, structures, and functions
#include "utilities.h" // Header file for miscellaneous utility functions


// Function to perform currency conversion
// Takes the 'amount' to be converted and the 'exchangeRate' as input
// Returns the converted amount after applying the exchange rate
double convertCurrency(double amount, double exchangeRate) {
    return amount * exchangeRate; // Calculates the converted amount
}


// Function to initialize and fetch supported currencies
// Fetches currency data from an API, parses the JSON response, and stores supported currency codes globally
void fetchSupportedCurrencies() {
    CURL *curl; // CURL session handle
    CURLcode res; // CURL operation result
    struct MemoryStruct chunk; // Structure to hold received data
    char errorBuffer[CURL_ERROR_SIZE]; // Buffer to store error messages

    // Initialize memory chunk
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init(); // Initialize CURL session
    if (curl) {
        // Set the API URL and key
        char url[200];
        sprintf(url, "%s?api_key=%s", URL_CURRENCY, API_KEY);

        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackForCurrency);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // Perform the API request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            // Display error message if request fails
            fprintf(stderr, "\n\t\t\t\t\t\t\tcurl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            if (errorBuffer[0] != '\0') {
                fprintf(stderr, "\n\t\t\t\t\t\t\tError: %s\n", errorBuffer);
            }
        } else {
            // Parse JSON response and store currency codes globally
            cJSON *json = cJSON_Parse(chunk.memory);
            if (json != NULL) {
                cJSON *currency = NULL;
                cJSON_ArrayForEach(currency, json) {
                    cJSON *code = cJSON_GetObjectItem(currency, "code");
                    if (code != NULL) {
                        // Increment the number of currencies and allocate memory for the new code
                        supportedCurrencies = realloc(supportedCurrencies, (numberOfCurrencies + 1) * sizeof(char*));
                        supportedCurrencies[numberOfCurrencies] = strdup(code->valuestring);
                        numberOfCurrencies++;
                    }
                }
                cJSON_Delete(json); // Clean up cJSON object
            } else {
                fprintf(stderr, "\n\t\t\t\t\t\t\tError parsing JSON\n");
            }
        }

        free(chunk.memory); // Free allocated memory
        curl_easy_cleanup(curl); // Cleanup CURL session
    }
}


// Function to fetch and display supported currencies
// Uses an API request to retrieve currency data and parses JSON response to display codes and names
void displaySupportedCurrencies() {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk; // Structure to hold received data
    char errorBuffer[CURL_ERROR_SIZE]; // Buffer to store error messages

    // Initialize memory chunk
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init(); // Initialize CURL session
    if (curl) {
        // Set the API URL and key
        char url[200];
        sprintf(url, "%s?api_key=%s", URL_CURRENCY, API_KEY);

        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackForCurrency);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

        // Perform the API request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            // Display error message if request fails
            fprintf(stderr, "\n\t\t\t\t\t\t\tcurl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            if (errorBuffer[0] != '\0') {
                fprintf(stderr, "\n\t\t\t\t\t\t\tError: %s\n", errorBuffer);
            }
        } else {
            // Parse JSON response and display currency codes and names
            cJSON *json = cJSON_Parse(chunk.memory);
            if (json != NULL) {
                cJSON *currency = NULL;
                cJSON_ArrayForEach(currency, json) {
                    cJSON *code = cJSON_GetObjectItem(currency, "code");
                    cJSON *name = cJSON_GetObjectItem(currency, "name");
                    if (code != NULL && name != NULL) {
                        SetConsoleOutputCP(CP_UTF8); // Set console to UTF-8 for proper character display
                        printf("\n\n\t\t\t\t\t\t\t%s - %s\n", code->valuestring, name->valuestring);
                    }
                }
                cJSON_Delete(json); // Clean up cJSON object
            } else {
                fprintf(stderr, "\n\t\t\t\t\t\t\tError parsing JSON\n");
            }
        }

        free(chunk.memory); // Free allocated memory
        curl_easy_cleanup(curl); // Cleanup CURL session
    }
}


// Perform currency conversion using FX Rates API with cJSON for JSON parsing
void performCurrencyConversion(double amount, const char* fromCurrency, const char* toCurrency, const char* date) {
    // Validate 'fromCurrency'
    if (!isValidCurrency(fromCurrency)) {
        fprintf(stderr, "\n\t\t\t\t\t\t\tError: Invalid 'from' currency code.\n\n");
        return;
    }

    // Validate 'toCurrency'
    if (!isValidCurrency(toCurrency)) {
        fprintf(stderr, "\n\t\t\t\t\t\t\tError: Invalid 'to' currency code.\n\n");
        return;
    }

    // Validate 'date' format
    if (!validateDateFormat(date)) {
        fprintf(stderr, "\n\t\t\t\t\t\t\tError: Invalid date format. Please use YYYY-MM-DD format.\n\n");
        return;
    }
	
	char currentDate[11]; // Buffer to store current date in YYYY-MM-DD format
    getCurrentDateUTC(currentDate); // Get current date in UTC

    // If the date is in the future, set it to the current date in UTC
    if (strcmp(date, currentDate) > 0) {
        printf("\n\t\t\t\t\t\t\tDate parameter adjusted to current UTC date.\n\n");
        date = currentDate;
    }
        
	
    char url[200];
    sprintf(url, URL_CONVERT, fromCurrency, toCurrency, date, amount);

    // Initialize CURL session
    CURL* curl = curl_easy_init();
    if (curl) {
        char response[RESPONSE_BUFFER_SIZE] = {0};
        CURLcode res;

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Disable SSL verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Set the write callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackForConversion);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        // Set authentication header
        struct curl_slist* headers = NULL;
        char authHeader[100];
        sprintf(authHeader, "\n\t\t\t\t\t\t\tAuthorization: Bearer %s", API_KEY);
        headers = curl_slist_append(headers, authHeader);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Make the API request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            // Handle request failure
            fprintf(stderr, "\n\t\t\t\t\t\t\tFailed to fetch exchange rates: %s\n\n", curl_easy_strerror(res));
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            return;
        }

        // Parse the API response using cJSON
        cJSON* jsonResponse = cJSON_Parse(response);
        if (jsonResponse) {
            // Check for API response success
            cJSON* success = cJSON_GetObjectItemCaseSensitive(jsonResponse, "success");
            if (cJSON_IsFalse(success)) {
                // Handle API error response
                cJSON* error = cJSON_GetObjectItemCaseSensitive(jsonResponse, "error");
                cJSON* description = cJSON_GetObjectItemCaseSensitive(jsonResponse, "description");
                if (cJSON_IsString(error) && cJSON_IsString(description)) {
                    fprintf(stderr, "\n\t\t\t\t\t\t\tFailed to fetch exchange rates.\n\n");
                    // Print error details
                    fprintf(stderr, "\n\t\t\t\t\t\t\tError Result:");
                    fprintf(stderr, "\n\t\t\t\t\t\t\t--------------------------------------------------");
                    fprintf(stderr, "\n\t\t\t\t\t\t\tError: %s\n", error->valuestring);
                    fprintf(stderr, "\n\t\t\t\t\t\t\tDescription: %s", description->valuestring);
                    fprintf(stderr, "\n\t\t\t\t\t\t\t--------------------------------------------------");

                    cJSON_Delete(jsonResponse); // Clean up cJSON object
                    curl_slist_free_all(headers);
                    curl_easy_cleanup(curl);
                    
                    clearInputBuffer(); // Clear the input buffer
                    
                    return;
                }
            }

            // Process the successful API response
            cJSON* info = cJSON_GetObjectItemCaseSensitive(jsonResponse, "info");
            if (cJSON_IsObject(info)) {
                cJSON* rate = cJSON_GetObjectItemCaseSensitive(info, "rate");
                if (cJSON_IsNumber(rate)) {
                    // Extract and calculate conversion details
                    double exchangeRate = rate->valuedouble;
                    double convertedAmount = convertCurrency(amount, exchangeRate);

                    // Print the conversion result
                    printf("\n\t\t\t\t\t\t\tConversion Result:");
                    printf("\n\t\t\t\t\t\t\t-----------------------------------------------------------------");
                    printf("\n\t\t\t\t\t\t\t%.2lf %s is equal to %.2lf %s on %s\n", amount, fromCurrency, convertedAmount, toCurrency, date);
                    printf("\n\t\t\t\t\t\t\tConverted: %.2lf %s = %.2lf %s", amount, fromCurrency, convertedAmount, toCurrency);
                    printf("\n\t\t\t\t\t\t\tExchange Rate: 1 %s = %.2lf %s", fromCurrency, exchangeRate, toCurrency);
                    printf("\n\t\t\t\t\t\t\tDate: %s\n", date);

                    // Display last updated time
                    displayLastUpdatedTime();

                    printf("\t\t\t\t\t\t\t-----------------------------------------------------------------");
                    
                    clearInputBuffer();

                    cJSON_Delete(jsonResponse); // Clean up cJSON object
                    curl_slist_free_all(headers);
                    curl_easy_cleanup(curl);
                    return;
                }
            }

            cJSON_Delete(jsonResponse); // Clean up cJSON object if parsing fails
        }

        // Handle failure to parse exchange rate data from API response
        fprintf(stderr, "\n\t\t\t\t\t\t\t--------------------------------------------------\n");
        fprintf(stderr, "\n\t\t\t\t\t\t\tFailed to parse exchange rate data from API response.\n");
        fprintf(stderr, "\n\t\t\t\t\t\t\tAPI Response: %s\n", response);
        fprintf(stderr, "\n\t\t\t\t\t\t\t--------------------------------------------------");

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return;
    }
}

