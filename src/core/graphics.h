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
    int size;

} Graphics;

// Create a graphics component
Graphics* createGraphics();

// Draw frame to the screen
void drawFrame(Graphics* g);

// Clear screen
void clearScreen(Graphics* g, uint8 color);

// Draw a line
void drawLine(Graphics* g, int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color);

// Fill a rectangle
void fillRect(Graphics* g, int16 x, int16 y, 
    int16 w, int16 h, uint8 col);

#endif // __GRAPHICS_H__
