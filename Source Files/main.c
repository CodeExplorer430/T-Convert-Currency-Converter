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


// Constants for URL format, API key, and buffer size
#define URL_CONVERT "https://api.fxratesapi.com/convert?from=%s&to=%s&date=%s&amount=%.2lf&format=json" // URL format for currency conversion
#define URL_CURRENCY "https://api.fxratesapi.com/currencies" // URL for fetching supported currencies
#define API_KEY "fxr_live_a98558fd39e8f499913f443c3285447dd320" // API key for accessing FX Rates API
#define RESPONSE_BUFFER_SIZE 4096 // Maximum size for response buffer
#define MAX_CURRENCIES 200 // Maximum number of supported currencies


// Define a global variable to store supported currency codes
char** supportedCurrencies; // Pointer to store an array of supported currency codes
int numberOfCurrencies = 0; // Variable to track the count of supported currency codes


// Struct to store response data and its size
struct MemoryStruct {
    char *memory;   // Pointer to store memory data (e.g., API response)
    size_t size;    // Size of the memory allocated for data storage
};


// Function to clear the input buffer
void clearInputBuffer() {
    int c;
    // Loop through and consume characters in the input buffer until encountering a newline or EOF
    while ((c = getchar()) != '\n' && c != EOF);
    // Note: EOF represents the end-of-file indicator
}


// Function to perform currency conversion
// Takes the 'amount' to be converted and the 'exchangeRate' as input
// Returns the converted amount after applying the exchange rate
static double convertCurrency(double amount, double exchangeRate) {
    return amount * exchangeRate; // Calculates the converted amount
}


// Callback function used to handle API response data retrieval
// Appends the received 'contents' to the user-provided memory 'userp'
// Size of 'contents' data is 'size * nmemb' bytes
// Returns the total size of the received data
static size_t writeCallbackForConversion(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb; // Calculate the actual size of the received data
    char* data = (char*)userp; // Cast user-provided memory to char pointer
    strcat(data, (char*)contents); // Append the received data to the user-provided memory
    return realsize; // Return the total size of the received data
}


// Callback function used during an API request to write received data
// Appends the 'ptr' data to the 'MemoryStruct' object 'data' and updates its size
// 'size' indicates the size of each data element, 'nmemb' is the number of elements received
// Returns the total size of the received data
static size_t writeCallbackForCurrency(void *ptr, size_t size, size_t nmemb, void *data) {
    size_t real_size = size * nmemb; // Calculate the actual size of the received data
    struct MemoryStruct *mem = (struct MemoryStruct *)data; // Cast user-provided data to MemoryStruct pointer

    // Reallocate memory to store the received data along with existing data
    mem->memory = realloc(mem->memory, mem->size + real_size + 1);

    if (mem->memory == NULL) {
        // Handle out-of-memory error
        fprintf(stderr, "\n\t\t\t\t\t\t\tNot enough memory (realloc returned NULL)\n");
        return 0; // Return 0 to indicate failure
    }

    // Copy the received data to the end of the existing memory block
    memcpy(&(mem->memory[mem->size]), ptr, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0; // Add null terminator to ensure proper string termination

    return real_size; // Return the total size of the received data
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


// Function to retrieve and display the current time in a formatted string
void displayLastUpdatedTime() {
    time_t rawtime;          // Variable to hold raw time data
    struct tm* timeinfo;     // Struct to store formatted time data
    char buffer[80];         // Buffer to hold formatted time string

    time(&rawtime);          // Get current time in raw format
    timeinfo = localtime(&rawtime); // Convert raw time to local time info

    // Format time as per the specified format and store in 'buffer'
    strftime(buffer, sizeof(buffer), "Last updated: %Y-%m-%d %H:%M %Z", timeinfo);

    // Display the formatted time
    printf("\n\n\t\t\t\t\t\t\t%s\n", buffer);
}


// Function to retrieve the current date and format it as YYYY-MM-DD
// Parameters:
// - currentDate: Pointer to a character array to store the formatted date
void getCurrentDate(char* currentDate) {
    time_t t = time(NULL);             // Get current time in seconds
    struct tm tm = *localtime(&t);     // Convert to local time structure

    // Format the current date as YYYY-MM-DD and store it in 'currentDate'
    sprintf(currentDate, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}


// Function to convert the input string to lowercase
// Parameters:
// - str: Pointer to the string to be converted to lowercase
void toLowercase(char* str) {
    while (*str) {
        *str = tolower((unsigned char)*str); // Convert each character to lowercase
        str++;                               // Move to the next character
    }
}


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


// Function to validate date format (YYYY-MM-DD)
bool validateDateFormat(const char* date) {
    // Check if the input string's length is not equal to 10 (YYYY-MM-DD has 10 characters)
    if (strlen(date) != 10) {
        return false; // Return false for an invalid length
    }

    // Iterate through each character in the input string
    for (int i = 0; i < 10; ++i) {
        // Check for positions where the delimiter '-' should be present (positions 4 and 7)
        if (i == 4 || i == 7) {
            // If the character is not a '-', return false for an invalid delimiter
            if (date[i] != '-') {
                return false; // Return false for an invalid delimiter
            }
        } else {
            // Check if the characters at positions other than the delimiter are not digits
            if (date[i] < '0' || date[i] > '9') {
                return false; // Return false for an invalid character (non-digit)
            }
        }
    }
    return true; // If all conditions pass, return true for a valid format
}


// Function to validate the date input
void validateDateInput(char* date) {
    while (true) {
        printf("\n\t\t\t\t\t\t\tEnter the date for exchange rates (YYYY-MM-DD or type 'today' for current date): \n");
        printf("\t\t\t\t\t\t\t> ");

        if (scanf("%10s", date) != 1) { // Read input and ensure it's not more than 10 characters
            printf("\n\t\t\t\t\t\t\tError: Invalid input.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        // Convert the user input to lowercase for case-insensitive comparison
        toLowercase(date);

        // Check if the user input is 'today' and fetch the current date
        if (strcmp(date, "today") == 0) {
            // Handle fetching current date here if needed
            getCurrentDate(date); // Get the current date in YYYY-MM-DD format
            printf("\n\n\t\t\t\t\t\t\tFetching current date...\n");
            break; // Break the loop for 'today' input
        }

        // Validate the entered date format
        if (validateDateFormat(date)) {
            break; // Valid date format, exit the loop
        } else {
            printf("\n\t\t\t\t\t\t\tError: Invalid date format. Please use YYYY-MM-DD format or type 'today'.\n");
        }
    }
}


// Function to get the current date in YYYY-MM-DD format (UTC)
void getCurrentDateUTC(char* currentDate) {
    time_t rawtime;         // Variable to hold the raw time (seconds since epoch)
    struct tm* timeinfo;    // Structure to hold time components

    time(&rawtime);         // Get current time in seconds since epoch

    timeinfo = gmtime(&rawtime); // Convert raw time to UTC time structure

    // Format the time according to YYYY-MM-DD and store it in 'currentDate'
    // strftime() formats time according to the specified format and stores it in the provided string
    strftime(currentDate, 11, "%Y-%m-%d", timeinfo);
    // '%Y' - Year with century as a decimal number
    // '%m' - Month as a decimal number (01-12)
    // '%d' - Day of the month as a decimal number (01-31)
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
        printf("\n\n\t\t\t\t\t\t\tAre you sure you want to exit? (Y/N): ");
        fgets(input, sizeof(input), stdin); // Read input

        // Remove newline character if present
        size_t length = strlen(input);
        if (input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        validInput = 1; // Assuming input is valid until proven otherwise

        if (strlen(input) == 0) {
            printf("\n\t\t\t\t\t\t\tInvalid input. Please enter 'Y' or 'N'.\n\n");
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
            printf("\n\t\t\t\t\t\t\tInvalid choice. Please enter 'Y' or 'N'.\n\n");
            validInput = 0; // Invalid input, continue loop to prompt again
        }
    } while (!validInput);

    return choice; // Return the chosen action (exit or return to main menu)
}

	
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
