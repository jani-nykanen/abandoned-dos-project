// Graphics
// (c) 2019 Jani Nykänen

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "types.h"

// Graphics type
typedef struct {

    // Framebuffer
    uint8* frame;
    // Framebuffer size
    int width;
    int height;

} Graphics;

// Create a graphics component
Graphics* createGraphics();

#endif // __GRAPHICS_H__
