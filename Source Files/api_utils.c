// api_utils.c - Source file for API-related functions

#include <stdio.h>      // Standard input-output library for basic I/O functions like printf and scanf
#include <stdlib.h>     // Standard library providing functions for memory allocation, random numbers, etc.
#include <string.h>     // Library for string manipulation functions like strlen, strcpy, etc.
#include <stddef.h> 	// Standard header defining types related to pointers and offsets, including NULL pointer
#include "api_utils.h"  // Header file containing API-related constants, structures, and functions


// Define the variable to store an array of supported currency codes
int numberOfCurrencies = 0; // Initializing the variable to track the count of supported currency codes to zero
char** supportedCurrencies; // This variable will hold the array of supported currency codes


// Callback function used to handle API response data retrieval
// Appends the received 'contents' to the user-provided memory 'userp'
// Size of 'contents' data is 'size * nmemb' bytes
// Returns the total size of the received data
size_t writeCallbackForConversion(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb; // Calculate the actual size of the received data
    char* data = (char*)userp; // Cast user-provided memory to char pointer
    strcat(data, (char*)contents); // Append the received data to the user-provided memory
    return realsize; // Return the total size of the received data
}


// Callback function used during an API request to write received data
// Appends the 'ptr' data to the 'MemoryStruct' object 'data' and updates its size
// 'size' indicates the size of each data element, 'nmemb' is the number of elements received
// Returns the total size of the received data
size_t writeCallbackForCurrency(void *ptr, size_t size, size_t nmemb, void *data) {
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