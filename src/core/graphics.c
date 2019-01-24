// Graphics
// (c) 2019 Jani Nykänen

#include "graphics.h"

#include <dos.h>
#include <conio.h>
#include <graph.h>

#include <stdlib.h>
#include <stdio.h>

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

    // Set video mode to 320x200 256 colors
    _setvideomode(_MRES256COLOR);
    // Set palette
    setPalette(g);

    return g;
}
