// Math extensions
// (c) 2019 Jani Nykänen

#ifndef __MATHEXT_H__
#define __MATHEXT_H__

#include "../core/types.h"

// Fixed-point precision
#define FIXED_PREC 128

// Negative modulo
int16 negMod(int16 m, int16 n);

// Fixed sine
int16 fixedSin(int16 angle);
// Fixed cosine
int16 fixedCos(int16 angle);

// Maximum & minimum for short
int16 maxInt(int16 a, int16 b);
int16 minInt(int16 a, int16 b);

// Absolute value
int16 extAbs(int16 a);

#endif //__MATHEXT_H__
