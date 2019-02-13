// Graphics
// (c) 2019 Jani Nykänen

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "types.h"
#include "bitmap.h"

// Graphics type
typedef struct {

    // Framebuffer
    uint8* frame;

    // Framebuffer size
    Vector2 frameDim;
    short frameSize; // Size in bytes

    // Viewport size
    Rect viewport;

} Graphics;

// Create a graphics component
Graphics* createGraphics();

// Destroy graphics
void destroyGraphics(Graphics* g);

// Draw frame to the screen
void gDrawFrame(Graphics* g);

// Clear screen
void gClearScreen(Graphics* g, uint8 color);

// Clear viewport
void gClearView(Graphics* g, uint8 color);

// Set viewport
void gSetViewport(Graphics* g, short x, short y, 
    short w, short h);
// Reset viewport
void gResetViewport(Graphics* g);

// Draw a line
void gDrawLine(Graphics* g, int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color);

// Fill a rectangle
void gFillRect(Graphics* g, int16 x, int16 y, 
    int16 w, int16 h, uint8 col);

// Draw a bitmap fast (= ignoring alpha)
void gDrawBitmapFast(Graphics* g, Bitmap* bmp, int16 x, int16 y);

#endif // __GRAPHICS_H__
