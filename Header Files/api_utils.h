// api_utils.h - Header file for API-related constants and structures

#ifndef API_UTILS_H
#define API_UTILS_H


// Constants for URL format, API key, and buffer size
#define URL_CONVERT "https://api.fxratesapi.com/convert?from=%s&to=%s&date=%s&amount=%.2lf&format=json" // URL format for currency conversion
#define URL_CURRENCY "https://api.fxratesapi.com/currencies" // URL for fetching supported currencies
#define API_KEY "fxr_live_a98558fd39e8f499913f443c3285447dd320" // API key for accessing FX Rates API
#define RESPONSE_BUFFER_SIZE 4096 // Maximum size for response buffer
#define MAX_CURRENCIES 200 // Maximum number of supported currencies


// Define a global variable to store supported currency codes
extern char** supportedCurrencies; // Pointer to store an array of supported currency codes
extern int numberOfCurrencies; // Variable to track the count of supported currency codes, declaration only


// Struct to store response data and its size
struct MemoryStruct {
    char *memory;   // Pointer to store memory data (e.g., API response)
    size_t size;    // Size of the memory allocated for data storage
};

// Callback function for handling conversion API responses
size_t writeCallbackForConversion(void* contents, size_t size, size_t nmemb, void* userp);

// Callback function for handling currency API responses
size_t writeCallbackForCurrency(void *ptr, size_t size, size_t nmemb, void *data);

#endif /* API_UTILS_H */
