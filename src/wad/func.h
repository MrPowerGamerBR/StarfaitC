#pragma once
#include "../starfaitbytebuffer.h"

typedef struct {
    StringPointer name;
    uint32_t occurenceCount;
    uint32_t firstAddress;
} Function;

typedef struct {
    size_t functionCount;
    Function* functions;
} FUNC;

FUNC FUNC_parse(StarfaitByteBuffer* buffer);