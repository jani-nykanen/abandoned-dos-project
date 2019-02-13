// Graphics
// (c) 2019 Jani Nykänen

#include "graphics.h"

#include <dos.h>
#include <conio.h>
#include <graph.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "palette.h"
#include "../util/mathext.h"

// Framebuffer size (we can asssume
// no other size is wanted)
#define FB_WIDTH 320
#define FB_HEIGHT 200

// VGA position
static const long VGA_POS = 0xA0000000;

// Palette constants
static const long PALETTE_INDEX = 0x03c8;
static const long PALETTE_DATA = 0x03c9;


// Clip a rectangle
static bool clipRect(Graphics* g, short* x, short* y, 
    short* w, short* h) {

    // Left
    if(*x < g->viewport.x) {

        *w -= g->viewport.x - (*x);
        *x = g->viewport.x;
    }
    // Right
    if(*x+*w >= g->viewport.x+g->viewport.w) {

        *w -= (*x+*w) - (g->viewport.x + g->viewport.w);
    }

    // Top
    if(*y < g->viewport.y) {

        *h -= g->viewport.y - (*y);
        *y = g->viewport.y;
    }
    // Bottom
    if(*y+*h >= g->viewport.y+g->viewport.h) {

        *h -= (*y+*h) - (g->viewport.y + g->viewport.h);
    }

    return *w > 0 && *h > 0;
}


// Clip (general)
static bool clip(Graphics* g, int16* sx, int16* sy, int16* sw, int16* sh, 
    int16* dx, int16* dy) {

    int16 ow, oh;

    // Left
    ow = *sw;
    if(*dx < g->viewport.x) {

        *sw -= g->viewport.x - (*dx);
        *sx += ow-*sw;
        *dx = g->viewport.x;
    }
    // Right
    if(*dx+*sw >= g->viewport.x+g->viewport.w) {

         *sw -= (*dx+*sw) - (g->viewport.x + g->viewport.w); 
    }

    // Top
    oh = *sh;
    if(*dy < g->viewport.y) {

        *sh -= g->viewport.y - (*dy);
        *sy += oh-*sh;
        *dy = g->viewport.y;
    }
    // Bottom
    if(*dy+*sh >= g->viewport.y+g->viewport.h) {

        *sh -= (*dy+*sh) - (g->viewport.y + g->viewport.h);
    }

    return *sw > 0 && *sh > 0;
}



// Set palette
static void setPalette(Graphics* g) {

    int16 i = 0;

    // Generate simple palette
    outp(PALETTE_INDEX,0);
    for(i = 0; i < 256*3 ;  ++ i) {

        outp(PALETTE_DATA, PALETTE[i]/4);
    }
}


// Create a graphics component
Graphics* createGraphics() {

    Graphics* g;

    // Allocate memory
    g = (Graphics*)malloc(sizeof(Graphics));
    if(g == NULL) {

        printf("malloc error.\n");
        return NULL;
    }

    // Create a framebuffer
    g->frame = (uint8*)malloc(sizeof(uint8)*FB_WIDTH*FB_HEIGHT);
    if(g->frame == NULL) {

        printf("malloc error.\n");
        return NULL;
    }

    // Set defaults
    g->frameDim.x = FB_WIDTH;
    g->frameDim.y = FB_HEIGHT;
    g->frameSize = g->frameDim.x*g->frameDim.y;
    // Clear to black
    gClearScreen(g, 0);

    // Set video mode to 320x200 256 colors
    _setvideomode(_MRES256COLOR);
    // Set palette
    setPalette(g);

    // Set default viewport
    g->viewport.x = 0;
    g->viewport.y = 0;
    g->viewport.w = FB_WIDTH;
    g->viewport.h = FB_HEIGHT;

    return g;
}


// Destroy graphics
void destroyGraphics(Graphics* g) {

    if(g == NULL) return;

    // Reset graphics mode
    _setvideomode( _DEFAULTMODE );

    // Free allocated data
    free(g->frame);
    free(g);
}


// Draw frame to the screen
void gDrawFrame(Graphics* g) {

    memcpy((void*)VGA_POS, (const void*)g->frame, g->frameSize);
}


// Clear screen
void gClearScreen(Graphics* g, uint8 color) {

    memset(g->frame, color, g->frameSize);
}


// Clear viewport
void gClearView(Graphics* g, uint8 color) {

    gFillRect(g, g->viewport.x, g->viewport.y,
        g->viewport.w, g->viewport.h, color);
}


// Set viewport
void gSetViewport(Graphics* g, short x, short y, 
    short w, short h) {

    g->viewport.x = x;
    g->viewport.y = y;
    g->viewport.w = w;
    g->viewport.h = h;
}


// Reset viewport
void gResetViewport(Graphics* g) {

    g->viewport.x = 0;
    g->viewport.y = 0;
    g->viewport.w = FB_WIDTH;
    g->viewport.h = FB_HEIGHT;
}


// Draw a line
void gDrawLine(Graphics* g, int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color) {

    int16 endx = g->viewport.x + g->viewport.w;
    int16 endy = g->viewport.y + g->viewport.h;

    int16 dx, dy;
    int16 sx = x1 < x2 ? 1 : -1;
    int16 sy = y1 < y2 ? 1 : -1; 

    int16 err;
    int16 e2;

    // Check if outside the screen
    if((x1 < 0 && x2 < 0) ||
       (y1 < 0 && y2 < 0) ||
       (x1 >= g->frameDim.x && x2 >= g->frameDim.x) ||
       (y1 >= g->frameDim.y && y2 >= g->frameDim.y)) {

        return;
    }

    // Compute error
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    err = (dx > dy ? dx : -dy) / 2;
     
    while(true) {

        // Put pixel
        if(y1 < endy && y1 >= g->viewport.y &&
            x1 < endx && x1 >= g->viewport.x)
            g->frame[y1 * g->frameDim.x + x1] = color;
        
        // Goal reached
        if (x1==x2 && y1==y2) 
            break;

        e2 = err;
        if (e2 >-dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}


// Fill a rectangle
void gFillRect(Graphics* g, int16 dx, int16 dy, 
    int16 w, int16 h, uint8 col) {

    int16 y;
    uint16 offset;

    // Clip
    if(!clipRect(g, &dx, &dy, &w, &h))
        return;

    // Draw
    offset = g->frameDim.x*dy + dx;
    for(y = dy; y < dy+h; ++ y) {

        memset(g->frame + offset, col, w);
        offset += g->frameDim.x;
    }

}


// Draw a bitmap fast (= ignoring alpha)
void gDrawBitmapFast(Graphics* g, Bitmap* bmp, int16 dx, int16 dy) {

    int16 y;
    int16 sx = 0;
    int16 sy = 0;
    int16 sw = bmp->width;
    int16 sh = bmp->height;
    uint16 offset;
    uint16 boff;

    if(g == NULL || bmp == NULL) return;

    // Clip
    if(!clip(g, &sx, &sy, &sw, &sh, &dx, &dy))
        return;

    // Copy lines
    offset = g->frameDim.x*dy + dx;
    boff = bmp->width*sy + sx;
    for(y = dy; y < dy+sh; ++ y) {

        memcpy(g->frame + offset, bmp->data + boff, sw);
        offset += g->frameDim.x;
        boff += bmp->width;
    }
}
