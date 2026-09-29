#pragma once
#include "arraylist_variable.h"
#include "variable.h"
#include "stringpointer.h"
#include "../starfaitbytebuffer.h"

typedef struct {
    uint32_t globalVariables;
    uint32_t instanceVariables;
    uint32_t localVariables;
    VariableArrayList* variables;
} VARIChunk;

VARIChunk VARIChunk_parse(StarfaitByteBuffer* buffer, size_t chunkSize);