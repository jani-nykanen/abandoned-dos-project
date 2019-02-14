// PNG-to-binary converter (rgb332 version)
// (c) 2018-2019 Jani Nykänen


#include <SDL2/SDL_image.h>

#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "stdbool.h"


// Load a bitmap & convert it to a binary format
int conv_bitmap(const char* in, const char* out, bool dither) {

    const Uint8 ALPHA = 170;
    const float DIVISOR = 36.428f;
    const int DIVISOR2 = 85;

    // Load surface
    SDL_Surface* surf = IMG_Load(in);
    if(surf == NULL) {
        
        printf("Failed to load a bitmap in %s\nERR: %s!\n",in, IMG_GetError());
        return 1;
    }

    unsigned int pixelCount = surf->w * surf->h;

    // Allocate image and temp buffer data
    Uint8* data = (Uint8*)malloc(sizeof(Uint8) * pixelCount);
    if(data == NULL) {
        
        exit(1);
    }

    // Go through the data
    int i = 0;
    int row = 0;
    int column = 0;
    Uint8 pixel;
    Uint8 r,g,b,a;
    Uint8 er,eg,eb;
    Uint8* pdata = (Uint8*)surf->pixels;
    Uint8 p = surf->format->BytesPerPixel;

    for(; i < pixelCount; i++) {

        row = i / surf->w;
        column = i % surf->w;

        if(p == 4) {

            a = pdata[i*4 +3];
            if(a < 255) {
                
                data[i] = ALPHA;
                continue;
            }
        }

        b = pdata[i*p +2];
        g = pdata[i*p +1];
        r = pdata[i*p ];

        // No dithering
        if(!dither) {

            er = (Uint8) round((float)r / DIVISOR);
            if(er > 7) r = 7;
            er = er << 5;
            eg = (Uint8) round((float)g / DIVISOR);
            if(eg > 7) eg = 7;
            eg = eg << 2;
            eb = (b / DIVISOR2);
        }
        // Dithering
        else {

            float (*func) (float);

            // Even
            if((row % 2 == 0 && column % 2 == 0) ||
              ((row % 2 == 1 && column % 2 == 1))) {

                func = floorf;
            }
            // Odd
            else {

                func = ceilf;
            }

            er = (Uint8) func(r / (DIVISOR/2.0f) );
            eg = (Uint8) func(g / (DIVISOR/2.0f) );
            eb = (Uint8) func(b / ((float)DIVISOR2 / 2.0f));

            er = er / 2;
            eg = eg / 2;
            eb = eb / 2;

            // Limit
            if(er > 7) er = 7;
            if(eg > 7) eg = 7;
            if(eb > 3) eb = 3;

            er = er << 5;
            eg = eg << 2;
        }

        pixel = er | eg | eb;

        data[i] = pixel;
    }

    // Save dimensions
    unsigned short w = (unsigned short)surf->w;
    unsigned short h = (unsigned short)surf->h;

    // Free surface
    free(surf);

    // Create output file
    FILE* f = fopen(out, "wb");
    if(f == NULL) {

        printf("Failed to create the file!\n");
        exit(1);
    }

    // Save dimensions
    fwrite(&w, sizeof(unsigned short), 1, f);
    fwrite(&h, sizeof(unsigned short), 1, f);

    // Save pixel data
    fwrite(data,sizeof(Uint8), w*h, f);

    // Close file
    fclose(f);

    return 0;
}


// Main
int main(int argc, char** argv) {

    // Check arguments
    if(argc < 3) {

        printf("Too few arguments! Help: bmpconv in out\n");
        return 1;
    }
    // Check if dither
    bool dither = false;
    if(argc > 3) {

        if(strcmp(argv[3], "-dither") == 0) {

            dither = true;
        }
    }

    // Init SDL2_img
    IMG_Init(IMG_INIT_PNG);

    // Convert
    return conv_bitmap(argv[1], argv[2], dither);
}
