#pragma once
#include "stringpointer.h"

typedef struct {
    StringPointer name;
    int32_t length;
    uint16_t localsCount;
    uint16_t argumentsCount;
    int32_t bytecodeRelativeOffsetFieldPosition;
    int32_t bytecodeRelativeOffset;
    int32_t offset;
} CodeEntry;
