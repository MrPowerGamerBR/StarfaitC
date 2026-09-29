#pragma once
#include "../../starfaitbytebuffer.h"

typedef struct {
    size_t address;
    char* string;
} StringWrapper;

typedef struct {
    size_t stringCount;
    StringWrapper** strings;
} STRGChunk;

STRGChunk STRGChunk_parse(StarfaitByteBuffer* buffer);
char* STRGChunk_getString(STRGChunk* strg, StringPointer pointer);