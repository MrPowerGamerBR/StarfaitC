#pragma once
#include "../starfaitbytebuffer.h"

typedef struct {
    size_t address;
    char* string;
} StringWrapper;

typedef struct {
    size_t stringCount;
    StringWrapper** strings;
} STRG;

STRG STRG_parse(StarfaitByteBuffer* buffer);
char* STRG_getString(STRG* strg, StringPointer pointer);