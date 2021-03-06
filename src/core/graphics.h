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
    uint16 frameSize; // Size in bytes

    // Viewport size
    Rect viewport;
    // Is clipping enabled
    bool clipping;
    // Translation 
    Vector2 tr;

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

// Toggle clipping
void gToggleClipping(Graphics* g, bool state);

// Translate
void gTranslate(Graphics* g, int16 x, int16 y);
// "Additive translation"
void gMove(Graphics* g, int16 x, int16 y);

// Draw a line
void gDrawLine(Graphics* g, int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color);

// Fill a rectangle
void gFillRect(Graphics* g, int16 x, int16 y, 
    int16 w, int16 h, uint8 col);

// Draw a rectangle that is not filled
void gDrawRect(Graphics* g, int16 x, int16 y, 
    int16 w, int16 h, uint8 col);

// Draw a bitmap fast (= ignoring alpha)
void gDrawBitmapFast(Graphics* g, Bitmap* bmp, int16 x, int16 y);

// Draw a bitmap region fast (= ignoring alpha)
void gDrawBitmapRegionFast(Graphics* g, Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy);

// Draw text fast (ignoring alpha)
void gDrawTextFast(Graphics* g, Bitmap* font, const char* text, 
    int16 x, int16 y, int16 xoff, int16 yoff, bool center);

// Draw a bitmap
void gDrawBitmap(Graphics* g, Bitmap* bmp, int16 x, int16 y,
    bool flip);    

// Draw a bitmap region
void gDrawBitmapRegion(Graphics* g, Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy,
    bool flip);    

// Draw a bitmap region, but skip some pixels
void gDrawBitmapRegionSkip(Graphics* g, Bitmap* bmp, 
    int16 sx, int16 sy, int16 sw, int16 sh, int16 dx, int16 dy,
    int16 skip, bool flip);    

#endif // __GRAPHICS_H__
