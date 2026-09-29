#pragma once
#include "../../starfaitbytebuffer.h"
#include "arraylist_background.h"

typedef struct {
    BackgroundArrayList* backgrounds;
} BGNDChunk;

BGNDChunk BGNDChunk_parse(StarfaitByteBuffer* buffer);
