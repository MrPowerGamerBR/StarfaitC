#pragma once
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    uint32_t instanceType;
    uint32_t varId;
    uint32_t occurrenceCount;
    uint32_t firstAddress;
} Variable;
