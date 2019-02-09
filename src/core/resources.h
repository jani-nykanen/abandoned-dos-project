// Resource package
// (c) 2019 Jani Nykänen

#ifndef __RESOURCES__
#define __RESOURCES__

#include "types.h"

#include <stdbool.h>

// Resource name length
#define RES_NAME_LENGTH 128
// Resource pack max size
#define RES_MAX 256

// Resource types
enum {

    ResBitmap = 0,
};

// Resource
typedef struct {

    // The actual resource
    void* data;
    // Name
    char name [RES_NAME_LENGTH];
    // Type
    uint8 type;

} Resource;


// Resource list
typedef struct {

    // Resources
    Resource res [RES_MAX];
    // Count
    uint16 count;

} ResourceList;


// Create an empty resource list
ResourceList* createEmptyResourceList();

// Get a resource by its name
void* rsGetResource(ResourceList* rs, const char* name);

// Add a bitmap
bool rsAddBitmap(ResourceList* rs, const char* path, const char* name);

// Destroy
void destroyResList(ResourceList* rs);

#endif // __RESOURCES__
