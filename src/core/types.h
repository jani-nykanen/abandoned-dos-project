// Utility types
// (c) 2019 Jani Nykänen

#ifndef __TYPES_H__
#define __TYPES_H__

// I forgot to include this otherwise
#include <stdbool.h>

// Fixed-point decimal precision
#define FIXED_PREC 128

// Macros
#define _DECL_CONSTRUCTOR(ret, type, name) ret name(type x, type y);

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef signed short   int16;

// 2-component vectors
typedef struct {
    
    int16 x;
    int16 y;

} Vector2;
typedef struct {
    
    uint16 x;
    uint16 y;

} Vector2u;

// Rectangle
typedef struct {

    int16 x, y;
    int16 w, h;

} Rect;

// Constructors
_DECL_CONSTRUCTOR(Vector2, int16, vec2);
_DECL_CONSTRUCTOR(Vector2u, uint16, vec2u);

#endif // __TYPES_H__
