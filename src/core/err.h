// Simple error handling
// (c) 2019 Jani Nykänen

#ifndef __ERR__
#define __ERR__

#include <stdbool.h>

#define ERR_MAX_LENGTH 128

// Initialize error handling
void errInit();

// Throw an error
void errThrowNoParam(const char* msg);
void errThrowParam1(const char* msg, const char* param);

// Get an error. Returns NULL
// if does not exist
char* errGet();

#endif // __ERR__
