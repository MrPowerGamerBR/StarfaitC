#pragma once
#include "../../starfaitbytebuffer.h"
#include "arraylist_path.h"

typedef struct {
    PathArrayList* paths;
} PATHChunk;

PATHChunk PATHChunk_parse(StarfaitByteBuffer* buffer);
