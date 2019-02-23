// Simple error handling
// (c) 2019 Jani Nykänen

#include "err.h"

#include <stdlib.h>
#include <stdio.h>

// Error buffer
static char errBuffer [ERR_MAX_LENGTH];
// Is an error thrown
static bool hasError;


// Initialize error handling
void errInit() {

    hasError = false;
}


// Throw an error
void errThrowNoParam(const char* msg) {

    snprintf(errBuffer, ERR_MAX_LENGTH, "%s", msg);
    hasError = true;
}
void errThrowParam1(const char* msg, const char* param) {

    snprintf(errBuffer, ERR_MAX_LENGTH, "%s%s", msg, param);
    hasError = true;
}


// Get an error. Returns NULL
// if does not exist
char* errGet() {

    if(!hasError) return NULL;
    return errBuffer;
}
