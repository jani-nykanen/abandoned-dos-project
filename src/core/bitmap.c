// Bitmap
// (c) 2019 Jani Nykänen

#include "bitmap.h"

#include <stdlib.h>
#include <stdio.h>


// Create a bitmap
Bitmap* createBitmap(uint16 w, uint16 h, uint8* data) {

    uint16 i = 0;

    // Allocate memory
    Bitmap* bmp = (Bitmap*)malloc(sizeof(Bitmap));
    if(bmp == NULL) {

        printf("Memory allocation error!\n");
        return NULL;
    }
    bmp->data = (uint8*)malloc(sizeof(uint8) * w * h);
    if(bmp->data == NULL) {

        printf("Memory allocation error!\n");
        return NULL;
    }

    // Copy data (if any)
    if(data != NULL) {

        for(; i < w*h; ++ i) {

            bmp->data[i] = data[i];
        }
    }

    // Store size
    bmp->width = w;
    bmp->height = h;

    return bmp;
}


// Load a bitmap
Bitmap* loadBitmap(const char* path) {

    uint16 w, h;
    Bitmap* bmp;

    // Open file
    FILE* f = fopen(path, "rb");
    if(f == NULL) {

        printf("Could not load a file in:\n%s", path);
        return NULL;
    }

    // Read size
    fread(&w, sizeof(uint16), 1, f);
    fread(&h, sizeof(uint16), 1, f);

    // Allocate memory
    bmp = createBitmap(w, h, NULL);
    if(bmp == NULL) {

        printf("Failed to create a bitmap!");
        return NULL;
    }

    // Read data
    fread(bmp->data, sizeof(uint8), w*h, f);

    // Close file
    fclose(f);

    return bmp;
}


// Destroy a bitmap
void destroyBitmap(Bitmap* bmp) {

    if(bmp == NULL)
        return ;

    // Free data
    if(bmp->data != NULL)
        free(bmp->data);
    // Free bitmap
    free(bmp);
}
