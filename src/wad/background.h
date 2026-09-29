#pragma once
#include <stdbool.h>

#include "stringpointer.h"
#include "texturepageentrypointer.h"

typedef struct {
    StringPointer name;
    bool transparent;
    bool smooth;
    bool preload;
    TexturePageEntryPointer texturePageEntryPointer;
} Background;
