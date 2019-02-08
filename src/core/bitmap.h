// Bitmap
// (c) 2019 Jani Nykänen

#ifndef __BITMAP__
#define __BITMAP__

#include "types.h"

// Bitmap type
typedef struct {

    // Dimensions
    uint16 width;
    uint16 height;

    // Pixels
    uint8* data;

} Bitmap;

// Create a bitmap
Bitmap* createBitmap(uint16 w, uint16 h, uint8* data);

// Load a bitmap
Bitmap* loadBitmap(const char* path);

// Destroy a bitmap
void destroyBitmap(Bitmap* bmp);

#endif // __BITMAP__
