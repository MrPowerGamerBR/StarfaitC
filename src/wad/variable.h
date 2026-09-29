#pragma once
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    uint32_t instanceType;
    int32_t varId;
    uint32_t occurrenceCount;
    int32_t firstAddress;
} Variable;
