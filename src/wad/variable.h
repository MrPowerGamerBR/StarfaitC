#pragma once
#include "stringpointer.h"

struct {
    StringPointer name;
    uint32_t instanceType;
    uint32_t varId;
    uint32_t occurrenceCount;
    uint32_t firstAddress;
} typedef Variable;
