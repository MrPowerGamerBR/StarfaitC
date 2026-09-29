#pragma once
#include "variable.h"
#include "stringpointer.h"
#include "../starfaitbytebuffer.h"

struct {
    uint32_t globalVariables;
    uint32_t instanceVariables;
    uint32_t localVariables;
    size_t variableCount;
    Variable* variables;
} typedef VARI;

VARI VARI_parse(StarfaitByteBuffer* buffer, size_t chunkSize);