#pragma once
#include "../starfaitbytebuffer.h"
#include "arraylist_function.h"



typedef struct {
    FunctionArrayList* functions;
} FUNCChunk;

FUNCChunk FUNCChunk_parse(StarfaitByteBuffer* buffer);