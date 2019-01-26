// Utility types
// (c) 2019 Jani Nykänen

#ifndef __TYPES_H__
#define __TYPES_H__

// I forgot to include this otherwise
#include <stdbool.h>

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef signed short   int16;

// Fixed-point vector
typedef struct {
    
    int16 x;
    int16 y;

} Vector2;

#endif // __TYPES_H__
