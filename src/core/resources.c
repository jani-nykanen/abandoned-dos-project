// Resource package
// (c) 2019 Jani Nykänen

#include "resources.h"

#include "bitmap.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "err.h"


// Create an empty resource list
ResourceList* createEmptyResourceList() {

    ResourceList* rs = (ResourceList*)
        malloc(sizeof(ResourceList));
    if(rs == NULL) {

        errThrowNoParam("Memory allocation error!");
        return NULL;
    }

    rs->count = 0;
    rs->res[0].data = NULL;

    return rs;
}


// Get a resource by its name
void* rsGetResource(ResourceList* rs, const char* name) {

    uint16 i;
    if(rs == NULL)
        return NULL;

    for(i = 0; i < rs->count; ++ i) {

        if(strcmp(rs->res[i].name, name) == 0) {

            return rs->res[i].data;
        }
    }
    return NULL;
}



// Add a bitmap
bool rsAddBitmap(ResourceList* rs, const char* path, const char* name) {

    Bitmap* bmp;
    if(rs == NULL || rs->count >= RES_MAX)
        return false;

    bmp = loadBitmap(path);
    if(bmp == NULL) {

        return false;
    }

    // Copy name
    snprintf(rs->res[rs->count].name, RES_NAME_LENGTH, "%s", name);

    // Add resource
    rs->res[rs->count].data = (void*)bmp;
    rs->res[rs->count].type = ResBitmap;
    ++ rs->count;
    
    return true;
}


// Destroy
void destroyResList(ResourceList* rs) {

    uint16 i;

    if(rs == NULL)
        return;

    for(i = 0; i < rs->count; ++ i) {

        switch(rs->res[i].type) {

        case ResBitmap:

            destroyBitmap((Bitmap*)rs->res[i].data);
            break;

        default:
            free(rs->res[i].data);
        };
    }
    // Destroy list itself
    free(rs);
}
