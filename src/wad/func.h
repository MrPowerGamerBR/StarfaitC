#pragma once
#include "../starfaitbytebuffer.h"
#include "arraylist_function.h"



typedef struct {
    FunctionArrayList* functions;
} FUNC;

FUNC FUNC_parse(StarfaitByteBuffer* buffer);