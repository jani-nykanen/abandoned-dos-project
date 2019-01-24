// Math extensions
// (c) 2019 Jani Nykänen

#include "mathext.h"

#include "trig.h"


// Negative modulo
int16 negMod(int16 m, int16 n) {

    if(m < 0) {

        return n - (-m % n) -1;
    }
    return m % n;
}


// Fixed sine
int16 fixedSin(int16 angle) {

    // Restrict to integers in [0,359]
    angle = negMod(angle, 360);

    // Check value from the table
    return _getSineTable() [angle];
}


// Fixed cosine
int16 fixedCos(int16 angle) {

    return fixedSin(angle + 180);
}
