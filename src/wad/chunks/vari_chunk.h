#pragma once
#include "arraylist_variable.h"
#include "../variable.h"
#include "../stringpointer.h"
#include "../../starfaitbytebuffer.h"

typedef struct {
    int32_t globalVariables;
    int32_t instanceVariables;
    int32_t localVariables;
    VariableArrayList* variables;
} VARIChunk;

VARIChunk VARIChunk_parse(StarfaitByteBuffer* buffer, int32_t chunkSize);