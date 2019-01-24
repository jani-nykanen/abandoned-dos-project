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

// Framebuffer size (we can asssume
// no other size is wanted)
#define FB_WIDTH 320
#define FB_HEIGHT 200

// VGA position
static const long VGA_POS = 0xA0000000;

// Palette constants
static const long PALETTE_INDEX = 0x03c8;
static const long PALETTE_DATA = 0x03c9;


// Set palette
void setPalette(Graphics* g) {

    const char* palette = _getPalette();
    int16 i = 0;

    // Generate simple palette
    outp(PALETTE_INDEX,0);
    for(i = 0; i < 256*3 ;  ++ i) {

        outp(PALETTE_DATA, palette[i]/4);
    }
}


// Create a graphics component
Graphics* createGraphics() {

    Graphics* g;

    // Allocate memory
    g = (Graphics*)malloc(sizeof(Graphics));
    if(g == NULL) {

        printf("malloc error.\n");
        exit(1);
    }

    // Create a framebuffer
    g->frame = (uint8*)malloc(sizeof(uint8)*FB_WIDTH*FB_HEIGHT);
    if(g->frame == NULL) {

        printf("malloc error.\n");
        exit(1);
    }

    // Set defaults
    g->width = FB_WIDTH;
    g->height = FB_HEIGHT;
    g->size = g->width*g->height;

    // Set video mode to 320x200 256 colors
    _setvideomode(_MRES256COLOR);
    // Set palette
    setPalette(g);

    return g;
}


// Draw frame to the screen
void drawFrame(Graphics* g) {

    memcpy((void*)VGA_POS, (const void*)g->frame, g->size);
}


// Clear screen
void clearScreen(Graphics* g, uint8 color) {

    memset(g->frame, color, g->size);
}


// Draw a line
void drawLine(Graphics* g, int16 x1, int16 y1, 
    int16 x2, int16 y2, uint8 color) {

    // (Bresenham's line algorithm)
    // TODO: Clip

    int16 dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int16 dy = abs(y2-y1), sy = y1<y2 ? 1 : -1; 
    int16 err = (dx > dy ? dx : -dy) / 2;
    int16 e2;
     
    while(true) {

        // Put pixel
        if(y1 < g->height-1 && y1 >= 0 &&
            x1 <  g->width-1 && x1 >= 0 )
            g->frame[y1 * g->width + x1] = color;
        
        if (x1==x2 && y1==y2) 
            break;

        e2 = err;
        if (e2 >-dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}
