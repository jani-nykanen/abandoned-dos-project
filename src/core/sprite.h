// An animated sprite
// (c) 2019 Jani Nykänen

#ifndef __SPRITE__
#define __SPRITE__

#include "types.h"
#include "bitmap.h"
#include "graphics.h"

// Sprite type
typedef struct {

    int16 width;
    int16 height; 
    int16 frame;
    int16 row;
    int16 count;

} Sprite;

// Create a sprite
Sprite sprCreate(int16 w, int16 h);

// Animate a sprite
void sprAnimate(Sprite* s, int16 row, int16 start, int16 end, 
    int16 speed, int16 step);

// Draw a sprite frame
void sprDrawFrame(Sprite* s, Graphics* g, Bitmap* bmp, 
    int16 frame, int16 row, 
    int16 x, int16 y, bool flip);

// Draw a sprite
void sprDraw(Sprite* s, Graphics* g, Bitmap* bmp, 
    int16 x, int16 y, bool flip);

#endif // __SPRITE__
