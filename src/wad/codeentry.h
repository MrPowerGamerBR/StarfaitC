#pragma once
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    uint32_t length;
    uint32_t localsCount;
    uint32_t argumentsCount;
    uint32_t bytecodeRelativeOffsetFieldPosition;
    uint32_t bytecodeRelativeOffset;
    uint32_t offset;
} CodeEntry;
