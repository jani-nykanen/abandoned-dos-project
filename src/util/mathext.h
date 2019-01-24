// Math extensions
// (c) 2019 Jani Nykänen

#ifndef __MATHEXT_H__
#define __MATHEXT_H__

#include "../core/types.h"

// Fixed-point precision
#define FIXED_PREC 128

// Fixed-point vector
typedef struct {
    
    int16 x;
    int16 y;

} Vector2;

// Negative modulo
int16 negMod(int16 m, int16 n);

// Fixed sine
int16 fixedSin(int16 angle);
// Fixed cosine
int16 fixedCos(int16 angle);

#endif //__MATHEXT_H__
