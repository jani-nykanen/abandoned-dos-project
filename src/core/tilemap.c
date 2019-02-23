// Tilemap
// (c) 2019 Jani Nykänen

#include "tilemap.h"

#include <stdio.h>
#include <stdlib.h>

#include "err.h"


// Load a tilemap
Tilemap* loadTilemap(const char* path) {

    Tilemap* t;
    FILE* f;
    uint8 i;

    // Allocate memory
    t = (Tilemap*)malloc(sizeof(Tilemap));
    if(t == NULL) {

        errThrowNoParam("Memory allocation error!");
        return NULL;
    }

    // Read file
    f = fopen(path, "rb");
    if(f == NULL) {

        errThrowParam1("Could not open a file in: ", path);
        return NULL;
    }

    // Read size
    fread(&t->width, sizeof(uint16), 1, f);
    fread(&t->height, sizeof(uint16), 1, f);
    fread(&t->layerCount, sizeof(uint8), 1, f);

    // Allocate memory for the layers
    t->layers = (uint8**)malloc(sizeof(uint8*) * t->layerCount);
    if(t->layers == NULL) {

        errThrowNoParam("Memory allocation error!");
        return NULL;
    }

    // Copy layers
    for(i = 0; i < t->layerCount; ++ i) {

        // Allocate
        t->layers[i] = (uint8*)malloc(sizeof(uint8) * t->width * t->height);
        if(t->layers[i] == NULL) {

            errThrowNoParam("Memory allocation error!");
            return NULL;
        }
        // Read
        fread(t->layers[i], sizeof(uint8), t->width*t->height, f);
    }

    // Close file
    fclose(f);

    return t;
}


// Get a tile
uint8 mapGetTile(Tilemap* t, uint8 layer, int16 x, int16 y) {

    if(t == NULL) return 0;

    // If out of range, return 0
    if(layer >= t->layerCount ||
       x < 0 || y < 0 || x >= (int16)t->width || y >= (int16)t->height)
       return 0;

    // Get tile
    return t->layers[layer][y*t->width + x];
}


// Destroy a tilemap
void destroyTilemap(Tilemap* t) {

    uint8 i = 0;
    if(t == NULL) return;

    // Destroy layers
    for(i = 0; i < t->layerCount; ++ i) {

        free(t->layers[i]);
    }
    free(t->layers);
    free(t);
}
