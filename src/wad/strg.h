#pragma once
#include "../starfaitbytebuffer.h"

struct {
    size_t address;
    char* string;
} typedef StringWrapper;

struct {
    size_t stringCount;
    StringWrapper** strings;
} typedef STRG;

STRG STRG_parse(StarfaitByteBuffer* buffer);
char* STRG_getString(STRG* strg, StringPointer pointer);