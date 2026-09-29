#pragma once
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    int32_t instanceType;
    int32_t varId;
    int32_t occurrenceCount;
    int32_t firstAddress;
} Variable;
