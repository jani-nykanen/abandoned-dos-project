// Utility types
// (c) 2019 Jani Nykänen

#include "types.h"

#define _DEF_CONSTRUCTOR(ret, type, name) \
    ret name(type x, type y) { \
        ret p; p.x = x; p.y = y; return p; \
} \

// Constructors (for vectors)
_DEF_CONSTRUCTOR(Vector2, int16, vec2)
_DEF_CONSTRUCTOR(Vector2u, uint16, vec2u)
