#pragma once
#include "../starfaitbytebuffer.h"

struct {
    StringPointer name;
    uint32_t occurenceCount;
    uint32_t firstAddress;
} typedef Function;

struct {
    size_t functionCount;
    Function* functions;
} typedef FUNC;

FUNC FUNC_parse(StarfaitByteBuffer* buffer);